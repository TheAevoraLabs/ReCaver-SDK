// ReCaver Embedded TCP Terminal Server
// Credit: Quantum Creeper / Aevora Labs
// Provides interactive Lua REPL and script streaming over TCP on desktop and mobile.

#ifndef CAVER_TERMINALSERVER_H
#define CAVER_TERMINALSERVER_H

#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>

struct lua_State;

namespace Caver {

class GameViewController;

namespace Terminal {

class TerminalServer {
public:
    static TerminalServer& Get();

    // Starts listening on first available port in [portStart, portEnd].
    bool Start(int portStart = 1337, int portEnd = 1345);
    void Stop();

    // Called once per frame on the main engine thread.
    void Tick(float deltaTime, GameViewController* viewController, lua_State* L);

    int GetPort() const { return m_port; }
    bool IsRunning() const { return m_running; }
    void Broadcast(const std::string& message);

private:
    TerminalServer();
    ~TerminalServer();
    TerminalServer(const TerminalServer&);
    TerminalServer& operator=(const TerminalServer&);

    void NetworkThreadFunc();
    void HandleClientData(int clientFd, const char* data, size_t size);
    void ExecuteCommand(const std::string& command, lua_State* L, GameViewController* viewController);

    std::atomic<bool> m_running;
    std::atomic<int> m_port;
    int m_serverFd;
    int m_activeClientFd;
    std::thread m_networkThread;

    std::mutex m_queueMutex;
    std::queue<std::string> m_commandQueue;
    std::string m_inputAccumulator;
    std::string m_currentSceneName;
    float m_fps;
};

} // namespace Terminal
} // namespace Caver

#endif // CAVER_TERMINALSERVER_H
