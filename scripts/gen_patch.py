"""
script inspired from:
https://github.com/skyline-dev/skyline/blob/53fdcf4491f445ce27105868a7d83d5da676e202/scripts/genPatch.py
"""
import os
import sys
import re
import struct
import json
import warnings
from typing import Optional, List, Dict, ClassVar
from glob import glob
from dataclasses import dataclass
from configparser import RawConfigParser

from keystone import (
    Ks,
    KS_ARCH_ARM64,
    KS_MODE_LITTLE_ENDIAN,
)

# consts
PATCH_DIR = "patches"
NSO_HEADER_LEN = 0x100
IPS_HEADER_MAGIC = bytes("IPS32", "ASCII")
IPS_EOF_MAGIC = bytes("EEOF", "ASCII")


@dataclass
class NintendoSwitchObject:
    """
    Also known as nso file
    """

    # handle NSO class var (in config order)
    __nso_data: ClassVar[Dict[str, "NintendoSwitchObject"]] = {}

    name: str
    address: int  # absolute runtime memory addr
    build_id: Optional[str] = None

    def __post_init__(self):
        """Adds current instance into NSO data class object"""
        # handle __symbol_data["foo(int)"]
        self._symbol_data = {}

        # pylint: disable=protected-access
        if self.name in self.__class__.__nso_data:
            raise Exception(f"NSO instance `{self.name}` is already defined.")
        self.__class__.__nso_data[self.name] = self

    @classmethod
    def get(cls, name) -> "NintendoSwitchObject":
        """Returns NSO from class object"""
        if name not in cls.__nso_data:
            raise Exception(f"NSO instance `{name}` is not defined.")
        return cls.__nso_data[name]

    @classmethod
    def list_names(cls) -> List[str]:
        """Returns current instances names (in creation order)"""
        # from python3.7 and greater
        # dict key insertion are preserved
        return list(cls.__nso_data.keys())

    @classmethod
    def list(cls) -> List["NintendoSwitchObject"]:
        """Returns current instances (in creation order)"""
        return list(cls.__nso_data.values())

    def add_symbol(self, symbol: "Symbol"):
        """
        Adds symbol data to the current nso object
        """
        if symbol.signature in self._symbol_data:
            warnings.warn(
                f"Symbol `{self.name}` `{symbol.signature}` is already defined."
            )
            return
        self._symbol_data[symbol.signature] = symbol

    def get_symbol(self, signature: str) -> "Symbol":
        """
        Returns symbol object form signature
        """
        if signature not in self._symbol_data:
            raise ValueError(f"Symbol `{self.name}` `{signature}` is not defined.")
        return self._symbol_data[signature]


@dataclass
class Symbol:
    """
    Handler for symbol offsets and signatures
    """

    nso: NintendoSwitchObject
    offset: int  # from nso
    signature: str

    def __post_init__(self):
        """Add current instance into NSO data class object"""
        self.nso.add_symbol(self)

    @property
    def address(self):
        """
        Returns global offset in memory
        """
        return self.nso.address + self.offset

    @property
    def nso_address(self):
        """
        Returns offset in nso file
        """
        print(f"sym {self.offset:X}")
        return NSO_HEADER_LEN + self.offset

    @classmethod
    def find(cls, signature: str, nso_name: str = None) -> Optional["Symbol"]:
        """
        Retrieve symbol from signature
        """
        keys = NintendoSwitchObject.list_names()
        if nso_name is not None:
            keys = [nso_name]

        for key in keys:
            nso = NintendoSwitchObject.get(key)
            try:
                # return first match
                return nso.get_symbol(signature)
            except ValueError:
                pass
        return None


@dataclass
class Patch:
    """
    Handle .ips objects
    """

    # handle patches
    __patches: ClassVar[Dict[str, List["Patch"]]] = {}

    nso: NintendoSwitchObject
    name: str
    offset: int
    content: str

    def __post_init__(self):
        """Add current instance to patches list"""
        # pylint: disable=protected-access
        if self.nso.build_id is None:
            raise Exception(
                f"Unable to create patch `{self.name}` "
                f"for unknown NSO build_id `{self.nso}"
            )

        patches = self.__class__.__patches.setdefault(
            self.nso.build_id,
            [],
        )
        patches.append(self)

    @classmethod
    def build_ips(cls, build_id) -> bytes:
        """Returns raw .ips file bytes"""
        if build_id not in cls.__patches:
            raise ValueError(f"No patches defined for `{build_id}` is not defined.")

        patches = IPS_HEADER_MAGIC
        for patch in cls.__patches[build_id]:
            patches += patch.ips
        patches += IPS_EOF_MAGIC
        return patches

    @property
    def ips(self) -> bytes:
        """
        Returns current patch in ips format
        """
        asm = self.asm
        return struct.pack(">I", self.nso_address) + struct.pack(">H", len(asm)) + asm

    @property
    def address(self):
        """
        Returns global offset in memory
        """
        return self.nso.address + self.offset

    @property
    def nso_address(self):
        """
        Returns global offset in nso file
        """
        return NSO_HEADER_LEN + self.offset

    def _parse(self, line: str) -> bytearray:
        """
        asm are processed line by line
        """
        # bytes patch
        try:
            return bytearray.fromhex(line)
        except ValueError:
            pass

        # string patch
        match = re.search(r'"(.+)"', line)
        if match:
            return bytearray(
                bytes(match.group(1), "utf-8").decode("unicode_escape") + "\0",
                "utf-8",
            )

        # asm patch
        match = re.match(
            r"(?P<mnemonic>BL?)\s+(?P<signature>[^\#]+$)", line, re.IGNORECASE
        )
        if match:
            mnemonic = match.group("mnemonic")
            signature = match.group("signature")
            symbol = Symbol.find(signature)
            if symbol is None:
                raise Exception(f"Symbol for `{signature}` not found")
            address = symbol.address - self.address
            line = f"{mnemonic} #{hex(address)}"

        match = re.match(
            r"(?P<addr>addr?)\s+(?P<signature>[^\#]+$)", line, re.IGNORECASE
        )
        if match:
            _ = match.group("addr")
            signature = match.group("signature")
            symbol = Symbol.find(signature)
            if symbol is None:
                raise Exception(f"Symbol for `{signature}` not found")
            address = symbol.address - self.address + 0x7100000000
            address = symbol.address - self.address + 0x7100000000
            return bytearray(address.to_bytes(8, "little"))

        keystone = Ks(KS_ARCH_ARM64, KS_MODE_LITTLE_ENDIAN)
        asm, _count = keystone.asm(line)
        return asm

    @property
    def asm(self) -> bytes:
        """
        Build assembly for the current object
        """
        ret = b""
        for value in self.content.split("\n"):
            ret += bytes(self._parse(value.strip()))
        return ret


def load_config(version, title_id):
    config_path = os.path.join(PATCH_DIR, version, title_id)
    # read global config ini file
    conf = RawConfigParser()
    # to allow case-sensitive keys
    conf.optionxform = lambda option: option
    conf.read(os.path.join(config_path, "patch.ini"))

    build_ids = dict(conf["build_id"])

    for name, address in conf["nso_load_addr"].items():
        NintendoSwitchObject(
            name=name,
            address=int(address, 0),
            build_id=build_ids.get(name),
        )

    for nso in NintendoSwitchObject.list():
        # read Il2CppDumper map
        # https://github.com/Perfare/Il2CppDumper file
        il2cpp_path = os.path.join(config_path, f"{nso.name}.json")
        try:
            with open(il2cpp_path, "r", encoding="utf-8") as file:
                script_methods = json.loads(file.read())["ScriptMethod"]
                for method in script_methods:
                    Symbol(
                        nso=nso,
                        signature=method["Signature"].rstrip(";"),
                        offset=method["Address"],
                    )
        except FileNotFoundError:
            print(f"Failed to read {il2cpp_path}")

    # parse /build/100/xxxx/*.map
    map_regex = re.compile(
        r"^(?P<offset>0x[0-9a-fA-F]+)\s+(?P<signature>[^0-9 ().:][^ ()]*(\s?\(.*\))?)$"
    )

    for built_hook_map in glob(f"./build/{version}/{title_id}/*.map"):
        filename = os.path.basename(built_hook_map)
        name = filename.split(".map")[0]
        nso = NintendoSwitchObject.get(name)

        with open(built_hook_map, "r", encoding="utf-8") as file:
            for line in file:
                line = line.strip()
                match = map_regex.match(line)
                if match:
                    Symbol(
                        nso=nso,
                        signature=match.group("signature"),
                        offset=int(match.group("offset"), 0),
                    )

    for nso in NintendoSwitchObject.list():
        patches = {}
        try:
            patches = dict(conf[nso.name])
        except KeyError:
            continue

        # read patch.ini
        for name, content in patches.items():
            sig = name
            local_offset = 0

            # parse func()+10= ...
            if "+" in sig:
                # parse +0xdeadbeef patch offset
                sig, local_offset = sig.split("+", 1)
                local_offset = int(local_offset, 0)

            symbol_to_patch = Symbol.find(sig)
            if symbol_to_patch is not None:
                addr_to_patch = symbol_to_patch.address + local_offset
            else:
                addr_to_patch = int(sig, 0) + local_offset
            Patch(
                nso=nso,
                name=name,
                offset=addr_to_patch,
                content=content,
            )


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: " + sys.argv[0] + " [version] [title_id] [output]")
        sys.exit(1)
    build_version = sys.argv[1]
    title_id = sys.argv[2]
    ips_dir = sys.argv[3]

    load_config(build_version, title_id)
    try:
        os.mkdir(ips_dir)
    except FileExistsError:
        pass

    for nso in NintendoSwitchObject.list():
        build_id = nso.build_id
        if not build_id:
            continue

        patch = Patch.build_ips(build_id)
        patch_path = os.path.join(ips_dir, f"{build_id}.ips")
        with open(patch_path, "wb") as file:
            file.write(patch)
