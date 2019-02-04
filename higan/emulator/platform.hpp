#pragma once

namespace higan {

struct Platform {
  virtual auto attach(Node) -> void {}
  virtual auto detach(Node) -> void {}
  virtual auto open(Node, string name, vfs::file::mode mode, bool required = false) -> vfs::shared::file { return {}; }
  virtual auto videoFrame(Node::Video, const uint32* data, uint pitch, uint width, uint height) -> void {}
  virtual auto audioFrame(const double* samples, uint channels) -> void {}
  virtual auto inputPoll(Node::Input) -> void {}
  virtual auto inputRumble(uint port, uint device, uint input, bool enable) -> void {}

  //deprecated functionality
  struct Load {
    Load() = default;
    Load(uint pathID, string option = "") : valid(true), pathID(pathID), option(option) {}
    explicit operator bool() const { return valid; }

    bool valid = false;
    uint pathID = 0;
    string option;
  };

  virtual auto path(uint id) -> string { return {}; }
  virtual auto open(uint id, string name, vfs::file::mode mode, bool required = false) -> vfs::shared::file { return {}; }
  virtual auto load(uint id, string name, string type, vector<string> options = {}) -> Load { return {}; }
  virtual auto inputPoll(uint port, uint device, uint input) -> int16 { return 0; }
  virtual auto dipSettings(Markup::Node node) -> uint { return 0; }
  virtual auto notify(string text) -> void {}
};

extern Platform* platform;

}

namespace higan::Object {
  auto PlatformAttach(shared_pointer<Node> node) -> void { if(platform && node->name) platform->attach(node); }
  auto PlatformDetach(shared_pointer<Node> node) -> void { if(platform && node->name) platform->detach(node); }
}
