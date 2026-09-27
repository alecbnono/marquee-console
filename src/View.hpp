#pragma once

#include <atomic>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

// Renders output. No knowledge of state or command parsing.
class View {
public:
    ~View();

    void showMessage(const std::string& message) const;
    void showAsciiArt(const std::string& text) const;
    void showHelp() const;
    void showPrompt() const;
    void showMarquee(const std::vector<std::string>& text, int speed);
    void stopMarquee();
    bool isMarqueeRunning() const;

private:
    std::atomic<bool> stopRequested_{false};
    std::atomic<bool> isRunning_{false};
    std::thread       marqueeThread_;
};