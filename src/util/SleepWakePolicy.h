#pragma once

#include <cstddef>
#include <cstdint>

namespace SleepWakePolicy {

enum class Resume : uint8_t {
  Splash,
  Silent,
  Network,
  SplashlessWake,
};

// What started a sleep. Quick Sleep is an explicit Power-button request for the
// Quick Resume screen; Timeout honours "Quick Resume on Timeout".
enum class SleepTrigger : uint8_t {
  Manual,
  Timeout,
  QuickSleep,
};

constexpr bool usesQuickResume(const SleepTrigger trigger, const bool quickResumeScreen,
                               const bool quickResumeAfterTimeout) {
  return trigger == SleepTrigger::QuickSleep || quickResumeScreen ||
         (trigger == SleepTrigger::Timeout && quickResumeAfterTimeout);
}

// Quick Sleep keeps a book page on screen. Anywhere else (Home, menus, a
// reader overlay) a frozen screen is not worth keeping, so sleep normally.
constexpr SleepTrigger quickSleepTrigger(const bool bookPageVisible) {
  return bookPageVisible ? SleepTrigger::QuickSleep : SleepTrigger::Manual;
}

constexpr bool hasValidSavedFrame(const bool exists, const size_t actualSize, const size_t expectedSize) {
  return exists && actualSize == expectedSize;
}

// A UC8279 X3 can skip its initial resync only after a saved Quick Resume frame
// has been verified. Other resume paths retain their existing behavior.
constexpr bool shouldInitializeSeamlessly(const Resume resume, const bool isUc8279X3, const bool hasValidFrame) {
  return resume != Resume::Splash && !(resume == Resume::SplashlessWake && isUc8279X3 && !hasValidFrame);
}

}  // namespace SleepWakePolicy
