

**FAST-NUCES Karachi**  
**Department of Computer Science**  
**Operating Systems – Spring 2025**  

---

### **Project Proposal**

**Project Title:**  
**Multi-Quality Parallel Video Streaming Server**

**Course:** Operating Systems  
**Instructor(s):** Ms. Saeeda & Ms. Khadija  
**Semester:** Spring 2025  

**Group Members:**  
- Emaan Arshad (23I2560)  
- Dania Khan (23K0072)  
- Tanisha Kataria (23K0067)  
- Zunaira Amjad (23K0013)

---

### **Problem Statement:**  
Modern video streaming platforms like YouTube and Netflix rely heavily on concurrent systems to serve video content to multiple users in different qualities. This process, if not synchronized correctly, can lead to problems such as corrupted streams, race conditions, and inefficient performance. Our project aims to simulate a multi-threaded video streaming server in C that demonstrates the effective use of Operating Systems concepts like the Producer-Consumer problem, Reader-Writer problem, and Dining Philosophers problem.

The server will concurrently stream different video qualities (240p, 480p, 720p) to five clients, demonstrating real-world parallel processing and synchronization techniques.

---

### **Objectives:**  
- Stream video chunks concurrently to multiple clients.
- Simulate multiple classical OS problems (Producer-Consumer, Reader-Writer, Dining Philosophers).
- Implement shared memory and synchronization using threads, semaphores, and mutexes.
- Display real-time logs and leaderboards for chunk consumption.
- Enhance understanding of concurrency control in multi-threaded systems.

---

### **Tools & Technologies:**  
- Programming Language: C (with POSIX threads)  
- Operating System: Ubuntu 20.04 (in VirtualBox)  
- Synchronization: Mutex, Semaphore, Shared Memory  
- Multimedia Tools: `ffmpeg`, `mpv` (media player)  
- Scripts: Shell Scripts (`setup.sh`, `cleanup.sh`, `chunker.sh`)  
- Debugging: GDB, Valgrind  

---

### **Project Architecture:**

1. **Chunk Generator**  
   - Uses `ffmpeg` to split video into chunks (.ts files) in 240p, 480p, and 720p.

2. **Producer Thread**  
   - Loads chunks into a shared bounded buffer with a capacity of 10.

3. **Consumer Threads**  
   - Five threads simulate five clients consuming different quality chunks from the buffer and playing them via `mpv`.

4. **FIFO Implementation**  
   - Named pipes connect the server to the media player for chunk playback.

5. **Logging and Leaderboard**  
   - Logs every read/write operation with timestamps.  
   - Generates a leaderboard summarizing chunks consumed by each consumer.

6. **Classical Problems Used:**
   - **Producer-Consumer** – Synchronizes chunk loading and reading.  
   - **Reader-Writer** – Ensures buffer reads and writes don’t overlap dangerously.  
   - **Dining Philosophers** – Simulates limited network bandwidth/resources.

---

### **Expected Outcomes:**
- A fully functional real-time video streaming server with synchronized concurrency.
- Accurate and informative log files and leaderboard statistics.
- Clear demonstration of classical OS synchronization problems in action.
- Source code that is educational, reusable, and robust.

---

### **Timeline:**

| Week      | Task Description                              |
|-----------|-----------------------------------------------|
| Week 1    | Topic finalization, chunker script creation    |
| Week 2    | Producer-Consumer implementation               |
| Week 3    | FIFO communication and playback integration    |
| Week 4    | Classical problem implementation and testing   |
| Week 5    | Logging, performance testing, documentation    |

---

### **Deliverables:**
- Source Code: `video_server.c`, `chunker.sh`, `setup.sh`, `cleanup.sh`
- Input Files: Sample videos and chunked files
- Output Files: `server_log.txt`, `leaderboard.txt`
- Project Documentation: Proposal, Readme, and final report

---

### **Conclusion:**  
This project aims to bridge the gap between theoretical operating systems concepts and their real-world application by simulating a video streaming server. It offers a comprehensive learning opportunity by integrating thread management, memory synchronization, inter-process communication, and classical OS problems. This not only enriches our understanding but prepares us for concurrent system challenges in the industry.

-
