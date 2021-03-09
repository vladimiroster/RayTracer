#pragma once

#include <chrono>
#include <iostream>
#include <memory>
#include <string>

namespace RayTracer {
  class Profiler {
  private:
    class Profile {
    public:
      Profile(std::ostream& os, const std::string& label) : _start(std::chrono::high_resolution_clock::now()), _os(os), _label(label) { }

      ~Profile() {
        auto stop = std::chrono::high_resolution_clock::now(); 
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - _start); 
        _os << _label << ": " << duration.count() << '\n'; 
      }

    private:
      std::chrono::steady_clock::time_point _start;
      std::ostream& _os;
      std::string _label;
    };

  public:
    Profiler(bool enabled = false, std::ostream& os = std::cerr) : _enabled(enabled), _os(os) {}

    std::unique_ptr<Profile> profile(const std::string& label) const {
      if (_enabled) {
        return std::make_unique<Profile>(_os, label);
      }
      return nullptr;
    }

  private:
    bool _enabled;
    std::ostream& _os;
  };

  inline const Profiler profiler(false, std::cerr);
} // namespace RayTracer