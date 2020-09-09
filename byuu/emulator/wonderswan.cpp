#include <ws/interface/interface.hpp>

struct WonderSwan : Emulator {
  WonderSwan();
  auto load() -> bool override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

struct WonderSwanColor : Emulator {
  WonderSwanColor();
  auto load() -> bool override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

struct PocketChallengeV2 : Emulator {
  PocketChallengeV2();
  auto load() -> bool override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

WonderSwan::WonderSwan() {
  interface = new higan::WonderSwan::WonderSwanInterface;
  medium = icarus::medium("WonderSwan");
  manufacturer = "Bandai";
  name = "WonderSwan";
}

auto WonderSwan::load() -> bool {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    port->allocate();
    port->connect();
  }

  return true;
}

auto WonderSwan::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  if(node->identity() == "System" && name == "boot.rom") {
    return vfs::memory::open(Resource::WonderSwan::Boot, sizeof Resource::WonderSwan::Boot);
  }

  if(node->identity() == "System" && name == "save.eeprom") {
    return {};
  }

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto saveRAMVolatile = document["game/board/memory(content=Save)/volatile"];

  if(name == "program.rom") {
    return vfs::memory::open(game.image.data(), programROMSize);
  }

  if(name == "save.ram" || name == "save.eeprom") {
    if(saveRAMVolatile) return {};
    auto location = locate(game.location, ".sav", settings.paths.saves);
    if(auto result = vfs::disk::open(location, mode)) return result;
  }

  if(name == "time.rtc") {
    auto location = locate(game.location, ".rtc", settings.paths.saves);
    if(auto result = vfs::disk::open(location, mode)) return result;
  }

  return {};
}

auto WonderSwan::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Y1"   ) mapping = virtualPad.x;
  if(name == "Y2"   ) mapping = virtualPad.y;
  if(name == "Y3"   ) mapping = virtualPad.lb;
  if(name == "Y4"   ) mapping = virtualPad.rb;
  if(name == "X1"   ) mapping = virtualPad.up;
  if(name == "X2"   ) mapping = virtualPad.right;
  if(name == "X3"   ) mapping = virtualPad.down;
  if(name == "X4"   ) mapping = virtualPad.left;
  if(name == "B"    ) mapping = virtualPad.a;
  if(name == "A"    ) mapping = virtualPad.b;
  if(name == "Start") mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}

WonderSwanColor::WonderSwanColor() {
  interface = new higan::WonderSwan::WonderSwanColorInterface;
  medium = icarus::medium("WonderSwan Color");
  manufacturer = "Bandai";
  name = "WonderSwan Color";
}

auto WonderSwanColor::load() -> bool {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    port->allocate();
    port->connect();
  }

  return true;
}

auto WonderSwanColor::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  if(node->identity() == "System" && name == "boot.rom") {
    return vfs::memory::open(Resource::WonderSwanColor::Boot, sizeof Resource::WonderSwanColor::Boot);
  }

  if(node->identity() == "System" && name == "save.eeprom") {
    return {};
  }

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto saveRAMVolatile = document["game/board/memory(content=Save)/volatile"];

  if(name == "program.rom") {
    return vfs::memory::open(game.image.data(), programROMSize);
  }

  if(name == "save.ram" || name == "save.eeprom") {
    if(saveRAMVolatile) return {};
    auto location = locate(game.location, ".sav", settings.paths.saves);
    if(auto result = vfs::disk::open(location, mode)) return result;
  }

  if(name == "time.rtc") {
    auto location = locate(game.location, ".rtc", settings.paths.saves);
    if(auto result = vfs::disk::open(location, mode)) return result;
  }

  return {};
}

auto WonderSwanColor::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Y1"   ) mapping = virtualPad.x;
  if(name == "Y2"   ) mapping = virtualPad.y;
  if(name == "Y3"   ) mapping = virtualPad.lb;
  if(name == "Y4"   ) mapping = virtualPad.rb;
  if(name == "X1"   ) mapping = virtualPad.up;
  if(name == "X2"   ) mapping = virtualPad.right;
  if(name == "X3"   ) mapping = virtualPad.down;
  if(name == "X4"   ) mapping = virtualPad.left;
  if(name == "B"    ) mapping = virtualPad.a;
  if(name == "A"    ) mapping = virtualPad.b;
  if(name == "Start") mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}

PocketChallengeV2::PocketChallengeV2() {
  interface = new higan::WonderSwan::PocketChallengeV2Interface;
  medium = icarus::medium("Pocket Challenge V2");
  manufacturer = "Benesse";
  name = "Pocket Challenge V2";
}

auto PocketChallengeV2::load() -> bool {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    port->allocate();
    port->connect();
  }

  return true;
}

auto PocketChallengeV2::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  if(node->identity() == "System" && name == "boot.rom") {
    return vfs::memory::open(Resource::WonderSwan::Boot, sizeof Resource::WonderSwan::Boot);
  }

  if(node->identity() == "System" && name == "save.eeprom") {
    return {};
  }

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto saveRAMVolatile = document["game/board/memory(content=Save)/volatile"];

  if(name == "program.rom") {
    return vfs::memory::open(game.image.data(), programROMSize);
  }

  if(name == "save.ram" || name == "save.eeprom") {
    if(saveRAMVolatile) return {};
    auto location = locate(game.location, ".sav", settings.paths.saves);
    if(auto result = vfs::disk::open(location, mode)) return result;
  }

  if(name == "time.rtc") {
    auto location = locate(game.location, ".rtc", settings.paths.saves);
    if(auto result = vfs::disk::open(location, mode)) return result;
  }

  return {};
}

auto PocketChallengeV2::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"    ) mapping = virtualPad.up;
  if(name == "Down"  ) mapping = virtualPad.down;
  if(name == "Left"  ) mapping = virtualPad.left;
  if(name == "Right" ) mapping = virtualPad.right;
  if(name == "Pass"  ) mapping = virtualPad.a;
  if(name == "Circle") mapping = virtualPad.b;
  if(name == "Clear" ) mapping = virtualPad.y;
  if(name == "View"  ) mapping = virtualPad.start;
  if(name == "Escape") mapping = virtualPad.back;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}
