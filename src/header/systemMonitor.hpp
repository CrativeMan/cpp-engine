#ifndef SYSMON_H
#define SYSMON_H

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

  struct MemoryInfo {
    size_t total;
    size_t used;
    size_t free;
  };

  MemoryInfo getRAMInfo() {
    struct sysinfo memInfo;
    sysinfo(&memInfo);

    MemoryInfo info;
    info.total = memInfo.totalram * memInfo.mem_unit;
    info.free = memInfo.freeram * memInfo.mem_unit;
    info.used = info.total - info.free;

    return info;
  }

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
  SystemMonitor() : lastFrameTime(std::chrono::high_resolution_clock::now()) {}

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
    ImGuiTreeNodeFlags flag =
        ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::TreeNodeEx("Ram", flag)) {
      // System RAM info
      MemoryInfo sysRAM = getRAMInfo();
      ImGui::Text("System RAM:");
      ImGui::Text("  Total: %.2f MB", sysRAM.total / (1024.0 * 1024.0));
      ImGui::Text("  Used:  %.2f MB", sysRAM.used / (1024.0 * 1024.0));
      ImGui::Text("  Free:  %.2f MB", sysRAM.free / (1024.0 * 1024.0));

      // Process RAM info
      size_t currentMemory = getProcessMemoryUsage();
      ImGui::Text("\nProcess RAM Usage:");
      ImGui::Text("  Current: %.2f MB", currentMemory / (1024.0 * 1024.0));
      ImGui::Text("  Peak:    %.2f MB", peakRAMUsage / (1024.0 * 1024.0));
      ImGui::Text("  Change:  %.2f MB",
                  (currentMemory - initialRAMUsage) / (1024.0 * 1024.0));
      ImGui::TreePop();
    }

    // CPU Usage
    double cpuUsage = getCPUUsage();
    ImGui::Text("\nCPU Usage: %.2f%%", cpuUsage);

    // Frame Time and FPS
    ImGui::Text("Frame Time: %.3fms", frameTime);
    ImGui::Text("FPS: %.3f", fps);
  }
};

#endif // SYSMON_H
