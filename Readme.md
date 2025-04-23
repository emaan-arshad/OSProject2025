

# 🎬 Multi-Quality Parallel Video Streaming Server  
*A Comprehensive Operating Systems Project on Synchronization & Concurrency*

**Course:** Operating Systems  
**Instructors:** Ms. Saeeda, Ms. Khadija  
**Term:** Spring 2025  

**Team Members:**
- Emaan Arshad (23I2560)  
- Dania Khan (23K0072)  
- Tanisha Kataria (23K0067)  
- Zunaira Amjad (23K0013)

---

## 🧠 Abstract

In the age of multimedia consumption, seamless video streaming is a vital component of modern systems. This project simulates a **parallel multi-threaded video streaming server** capable of handling real-time playback at multiple resolutions (240p, 480p, 720p) across concurrent clients. Leveraging **POSIX threads**, **mutexes**, **semaphores**, and **FIFOs**, the system is a practical demonstration of classical Operating Systems synchronization problems: **Producer-Consumer**, **Reader-Writer**, and **Dining Philosophers**. We further integrated robust logging, chunk management, and client monitoring to emulate real-world media services. This project serves as a complete case study in concurrency control and synchronization.

---

## 🎯 Objectives

The project aimed to:

- **Design a thread-safe video server** capable of handling multiple clients concurrently without data races or inconsistencies.
- **Simulate real-time streaming scenarios** using classic OS synchronization patterns to model resource contention and thread interaction.
- **Enable adaptive media delivery** via multi-resolution support, buffering strategies, and thread coordination.
- **Incorporate logging and leaderboard tracking** to visualize thread activity and performance.
- **Build a modular and extensible system** that can be scaled or enhanced for more advanced scenarios like networked streaming.

---

## 🚀 Core Features

### 📦 Video Chunking & Quality Levels

Using `ffmpeg`, each input video is segmented into multiple `.ts` chunks of 2 seconds. This preprocessing step ensures:

- Manageable data units for threaded processing.
- Parallel access to video chunks without decoding overhead.
- Simultaneous generation of 240p, 480p, and 720p formats stored separately in a structured directory hierarchy.

> **Directory Structure Example:**  
> `video_chunks/240p/`, `video_chunks/480p/`, `video_chunks/720p/`

---

### 🔄 Producer-Consumer Implementation

- The **producer thread** is responsible for reading chunks from disk and loading them into a **shared  buffer** .
- Multiple **consumer threads** retrieve these chunks and stream them to corresponding clients through **named FIFOs**.
- Semaphores (`empty`, `full`) and a mutex ensure thread-safe buffer access, maintaining **mutual exclusion**, **deadlock avoidance**, and **bounded buffer coordination**.

---

### 📺 Client Playback via `mpv`

- Each client is represented by a separate thread and has a unique **named FIFO** (e.g., `/tmp/client0.fifo`).
- `mpv`, a lightweight video player, reads the FIFO as a real-time stream.
- Consumers write chunk data to FIFOs, simulating a live feed.
- Up to 5 clients are supported concurrently, each capable of selecting a resolution.

---

### 📜 Logging & Leaderboard

- Every significant action (chunk read, chunk streamed, FIFO write, etc.) is timestamped and written to `server_log.txt`.
- On termination, a **leaderboard** is generated showing the number of chunks each consumer processed.
- This provides visibility into load balancing, consumer performance, and system responsiveness.

Example:
```plaintext
[2025-04-23 20:10:47] Consumer 1 streamed chunk_0003_480p.ts
```

---

### ⚙️ Classical OS Problem Integration

1. **Producer-Consumer:**  
   Prevents race conditions when multiple threads interact with the buffer. Implemented with semaphores (`sem_wait`, `sem_post`) and mutex locks.

2. **Reader-Writer:**  
   Ensures multiple consumers can read from the buffer without interfering with the producer. Uses read-write locks to coordinate access.

3. **Dining Philosophers:**  
   Used to simulate network bandwidth/resource contention. Consumers must "acquire forks" (streaming slots) before writing to FIFOs, limiting simultaneous writes to avoid FIFO clashes and simulate streaming bottlenecks.

---

## 👥 Roles & Contributions

| Team Member        | Key Contributions                                                                      |
|--------------------|-----------------------------------------------------------------------------------     |
| **Emaan Arshad**   | Thread management, buffer synchronization, Fifo and mpv handling, Reader Writer Problem|
| **Dania Khan**     | Report and Testing, leader_board calculation  ,libraries setup                                         |
| **Tanisha Kataria**| Chunk preprocessing, buffer management, implementation of Dining Philosophers          |
| **Zunaira Amjad**  | Thread lifecycle (create/join), log formatting, script automation, UI polish           |

---

## 🔧 Tools & Technologies

| Category          | Tool / Technology             | Purpose                                          |
|-------------------|-------------------------------|--------------------------------------------------|
| Programming       | **C (POSIX Threads)**         | Core logic, threading, and synchronization       |
| Media Processing  | `ffmpeg`                      | Chunking and resolution conversion               |
| Playback          | `mpv`                         | Real-time media playback from FIFO               |
| OS Utilities      | FIFOs (`mkfifo`)              | IPC for producer-consumer communication          |
| Scripting         | Bash                          | Automation of setup, chunking, and cleanup       |
| Debugging         | GDB, Valgrind                 | Thread debugging and memory leak detection       |
| Platform          | Linux (Ubuntu VM)             | Development and testing environment              |

---

## 📂 File Structure Overview

```plaintext
.
├── video_server.c             # Main server code with all threads and synchronization
├── setup.sh                   # Sets up FIFOs, launches server, starts players
├── cleanup.sh                 # Kills all processes, deletes FIFOs, clears memory
├── video_chunker.sh           # Uses ffmpeg to chunk all videos into 3 quality levels
├── server_log.txt             # Contains timestamped runtime logs
├── leaderboard.txt            # Final performance of each consumer
├── input_videos/              # Raw input videos (v1.mp4 – v5.mp4)
└── video_chunks/              # Organized by resolution, stores .ts chunks
```

---

## 🛠️ Challenges & Engineering Solutions

| Problem                            | Root Cause / Description                                              | Solution Implemented                                         |
|-----------------------------------|------------------------------------------------------------------------|--------------------------------------------------------------|
| **Early FIFO closure**            | Consumers exited before final read completed                          | Added condition checks and delays before termination         |
| **mpv crashes (FIFO underflow)**  | FIFO read initiated before data write completed                       | Introduced controlled sleeps and semaphores                  |
| **ZINK / MESA rendering issues**  | VM graphics limitations causing console errors                         | Confirmed functional irrelevance; safe to ignore             |
| **Residual FIFOs after crashes**  | Manual FIFO removal became tedious                                     | Created `cleanup.sh` to auto-remove FIFOs and kill players   |

---

## 🧪 Sample Output Logs

```plaintext
[2025-04-23 20:10:41.293136] Producer loaded chunk 0_240p.ts
[2025-04-23 20:10:47.008843] Consumer 0 streamed chunk 1_240p.ts
[2025-04-23 20:10:49.543835] Consumer 0 streamed chunk 2_240p.ts
```

### 📊 Sample Leaderboard

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

## 🏁 Execution Instructions

```bash
chmod +x setup.sh cleanup.sh video_chunker.sh
./video_chunker.sh        # Preprocess input videos into chunks
./setup.sh                # Start server and all playback clients
# After watching the stream...
./cleanup.sh              # Cleanup FIFOs and terminate all threads
```

---

## ✨ Future Scope & Enhancements

- **Socket-based Streaming:** Extend the simulation to a network model with actual client-server socket communication.
- **Adaptive Bitrate Control:** Dynamically switch quality based on simulated bandwidth conditions.
- **Interactive Controls:** Enable rewind, pause, and resume features per consumer.
- **Graphical Monitoring Interface:** Real-time dashboards showing consumer activity and buffer states.
- **Fault Recovery:** Auto-restart consumers if a crash or disconnection is detected.
- **AI Integration:** Use machine learning to predict optimal quality per consumer based on usage patterns.

---

## 📝 Conclusion

This project embodies the essence of operating system concurrency concepts in a tangible, multimedia-rich application. By successfully modeling synchronization patterns in a realistic streaming context, we deepened our understanding of multi-threading, IPC, and system design. The simulation stands as a testament to the versatility of OS principles when applied beyond theoretical boundaries.

---

## 🏅 Acknowledgements

We extend heartfelt thanks to **Ms. Saeeda** and **Ms. Khadija** for their encouragement, insights, and continuous guidance throughout the semester. Their mentorship played a pivotal role in transforming this concept into a functional, feature-rich system.

Special mention to the **POSIX API**, **ffmpeg**, and **mpv** open-source communities, whose tools made this project possible.

