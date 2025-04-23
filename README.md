
# 🎬 Multi-Quality Parallel Video Streaming Server  
*An Operating Systems Project on Synchronization & Concurrency*

**Course:** Operating Systems  
**Instructor:** Ms. Saeeda and Ms. Khadija  
**Team Members:**
- Emaan Arshad (23I2560)  
- Dania Khan (23K0072)  
- Tanisha Kataria (23K0067)  
- Zunaira Amjad (23K0013)  

---

## 🧠 Abstract  
This project simulates a **real-time multi-threaded video streaming server** using the **Producer-Consumer paradigm**, incorporating classical OS synchronization problems such as **Reader-Writer** and **Dining Philosophers**. It handles multiple video resolutions (240p, 480p, 720p), chunking, and playback with real-time monitoring, logging, and concurrency management using **POSIX threads**, **mutexes**, **semaphores**, and **FIFOs**.

---

## 🎯 Objectives

- Enable smooth multi-client video streaming.  
- Prevent race conditions using mutexes and semaphores.  
- Model real-world synchronization issues in streaming scenarios.  
- Demonstrate classical OS problems (Producer-Consumer, Reader-Writer, Dining Philosophers).  
- Provide detailed logs and a consumer leaderboard for insight into thread behavior.

---

## 🚀 Features

### 📦 Video Chunking & Quality Levels
- `ffmpeg` splits 5 videos into 2-second `.ts` chunks.  
- Chunks generated in **240p**, **480p**, and **720p** resolutions.

### 🔄 Producer-Consumer Design
- **Producer** loads chunks into a **bounded buffer** (size 10).  
- **Consumers** read chunks and stream them via **FIFOs** to `mpv`.  
- Implements semaphores and mutexes for synchronization.

### 📺 Streaming with `mpv`
- Each FIFO is attached to a separate `mpv` player instance.  
- Supports concurrent playback for up to 5 clients.

### 📜 Logging & Leaderboard
- Logs timestamps for key actions (loading, playback, FIFO access).  
- Consumer activity logged in `server_log.txt`.  
- Final chunk counts per consumer displayed in `leaderboard.txt`.

### ⚙️ Classic OS Problem Integration
- **Reader-Writer**: Prevents writes during concurrent reads.  
- **Dining Philosophers**: Limits simultaneous streams to simulate resource contention.

### 🧠 Additional Enhancements
- Dynamic chunk sorting and parsing.  
- Graceful consumer termination after stream completion.  
- Persistent logs and statistics after shutdown.

---

## ⚔️ Challenges & Solutions

| Challenge                          | Solution                                                                 |
|-----------------------------------|--------------------------------------------------------------------------|
| FIFO closed before read completed | Added delays and condition checks for safe consumer termination          |
| `mpv` crashing due to FIFO timing | Implemented strict semaphore handling and delay-based synchronization    |
| ZINK / MESA errors in VM          | Noted these were harmless to functionality; continued in virtual setup   |
| Improper FIFO cleanup             | Created `cleanup.sh` to terminate all `mpv` processes and remove FIFOs   |

---

## 👥 Roles & Contributions

| Member             | Responsibilities                                                                 |
|--------------------|----------------------------------------------------------------------------------|
| Emaan Arshad       | Core logic (threads, mutex, semaphore), leaderboard, logging, shell scripting    |
| Dania Khan         | `mpv` playback, consumer implementation, Reader-Writer logic                     |
| Tanisha Kataria    | Chunk buffering, producer logic, Dining Philosophers constraints                 |
| Zunaira Amjad      | Thread creation/joining, final formatting and UI logs                            |

---

## 🔧 Technologies & Tools

- **Language:** C with POSIX PThreads  
- **Media Tools:** `ffmpeg`, `mpv`  
- **Platform:** Linux Ubuntu (VirtualBox)  
- **Debugging:** GDB, Valgrind  
- **Automation:** Bash Scripts (`setup.sh`, `cleanup.sh`, `video_chunker.sh`)  

---

## 📂 File Structure

```plaintext
.
├── video_server.c             # Main C program
├── setup.sh                   # Initializes FIFOs, server, and players
├── cleanup.sh                 # Terminates processes and cleans up
├── video_chunker.sh           # Creates video chunks at three resolutions
├── server_log.txt             # Log file for runtime operations
├── leaderboard.txt            # Consumer chunk stats
├── input_videos/              # Original videos (v1.mp4 – v5.mp4)
└── video_chunks/              # All chunked outputs per resolution
```

---

## 🧪 Sample Log Output

```plaintext
[2025-04-23 20:10:41.293136] Producer loaded chunk 0
[2025-04-23 20:10:47.008843] Consumer 0 processed chunk 1
[2025-04-23 20:10:49.543835] Consumer 0 processed chunk 2
```

### 📊 Leaderboard

```
==================== LEADERBOARD ====================
| Consumer   | Chunks Processed |
-----------------------------------------------------
| 0          | 3                 |
| 1          | 9                 |
| 2          | 5                 |
| 3          | 19                |
| 4          | 3                 |
=====================================================
```

---

## 🏁 How to Run

```bash
chmod +x setup.sh cleanup.sh video_chunker.sh
./video_chunker.sh      # Prepares video chunks
./setup.sh              # Starts server & player instances
# After playback...
./cleanup.sh            # Terminates all processes and removes FIFOs
```

---

## ✨ Future Scope

- 🛰️ Real-time network streaming via sockets  
- 📶 Adaptive bitrate selection based on simulated network conditions  
- 📈 GUI dashboard for monitoring consumers and chunk status  
- 🔁 Consumer rewind, pause, or skip options  
- 🤖 AI-based quality recommendations per consumer  
- 🛡️ Fault tolerance for failed consumers  

---

## 📝 Conclusion

This project successfully demonstrates how synchronization, inter-process communication, and classical OS problems apply to real-world systems. The result is a robust, thread-safe streaming server capable of real-time concurrent playback with detailed performance metrics and consumer monitoring.

---

## 🏅 Acknowledgements

Special thanks to **Ms. Saeeda** and **Ms. Khadija** for their guidance and support throughout the project.  
Thanks to open-source tools: **POSIX Threads**, **ffmpeg**, and **mpv** for enabling this simulation.

---
