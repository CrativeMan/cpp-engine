#ifndef SYSMON_HPP
#define SYSMON_HPP

#include "../../include/logger.hpp"
#include "imgui.h"
#include <chrono>
#include <fstream>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <unistd.h>

class SystemMonitor {
private:
  // For frame time calculation
  std::chrono::high_resolution_clock::time_point lastFrameTime;
  float frameTime = 0.0f;
  float fps = 0.0f;

  // For CPU usage calculation
  unsigned long long lastTotalUser = 0, lastTotalUserLow = 0, lastTotalSys = 0;
  unsigned long long lastTotalIdle = 0;

  // For memory tracking
  size_t peakRAMUsage = 0;
  size_t initialRAMUsage = 0;
  bool initializedRAM = false;

  double getCPUUsage() {
    std::ifstream statFile("/proc/stat");
    std::string line;
    getline(statFile, line);

    unsigned long long totalUser, totalUserLow, totalSys, totalIdle;

    sscanf(line.c_str(), "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
           &totalSys, &totalIdle);

    if (lastTotalUser == 0) {
      lastTotalUser = totalUser;
      lastTotalUserLow = totalUserLow;
      lastTotalSys = totalSys;
      lastTotalIdle = totalIdle;
      return 0.0;
    }

    unsigned long long diffUser = totalUser - lastTotalUser;
    unsigned long long diffUserLow = totalUserLow - lastTotalUserLow;
    unsigned long long diffSys = totalSys - lastTotalSys;
    unsigned long long diffIdle = totalIdle - lastTotalIdle;

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    unsigned long long diffTotal = diffUser + diffUserLow + diffSys + diffIdle;
    return (diffTotal - diffIdle) * 100.0 / diffTotal;
  }

  // Get process-specific memory usage
  size_t getProcessMemoryUsage() {
    std::ifstream statm("/proc/self/statm");
    long pages = 0;
    statm >> pages;
    return pages * sysconf(_SC_PAGESIZE);
  }

public:
  SystemMonitor() : lastFrameTime(std::chrono::high_resolution_clock::now()) {
    Logger::info("SystemMonitor", "SystemMonitor initialized");
  }

  void update() {
    // Update frame time and FPS
    auto currentTime = std::chrono::high_resolution_clock::now();
    frameTime =
        std::chrono::duration<float, std::milli>(currentTime - lastFrameTime)
            .count();
    fps = 1000.0f / frameTime;
    lastFrameTime = currentTime;

    // Track peak memory usage
    size_t currentMemory = getProcessMemoryUsage();
    if (!initializedRAM) {
      initialRAMUsage = currentMemory;
      initializedRAM = true;
    }
    peakRAMUsage = std::max(peakRAMUsage, currentMemory);
  }

  void render() {
    // Process RAM info
    size_t currentMemory = getProcessMemoryUsage();
    ImGui::Text("Process RAM Usage:");
    ImGui::Text("  Current: %.2f MB", currentMemory / (1024.0 * 1024.0));
    ImGui::Text("  Peak:    %.2f MB", peakRAMUsage / (1024.0 * 1024.0));
    ImGui::Text("  Change:  %.2f MB",
                (currentMemory - initialRAMUsage) / (1024.0 * 1024.0));

    // CPU Usage
    double cpuUsage = getCPUUsage();
    ImGui::Text("CPU Usage: %.2f%%", cpuUsage);

    // Frame Time and FPS
    ImGui::Text("Frame Time: %.3fms", frameTime);
    ImGui::Text("FPS: %.0f", fps);
  }
};

#endif // SYSMON_HPP
