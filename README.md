## WikiLens

WikiLens is a simple Wikipedia search engine built in C++ (backend) with a React frontend.

### Backend (C++)
- **Requirements**: CMake, a C++20 compiler, `expat`, `libasio-dev`, and Crow (included as a git submodule).
- **Build**:
  - `cd backend`
  - `git submodule update --init --recursive`
  - `cmake -S . -B build`
  - `cmake --build build`
- **Run**:
  - Make sure you have the Simple English Wikipedia dump at `simplewiki-latest-pages-articles.xml.1` in the project root.
  - `./build/wikilens`
  - Backend will listen on `http://localhost:8080`.

### Frontend (React)
- **Install & run**:
  - `cd frontend`
  - `bun install` (or `npm install` if you switch)
  - `bun run dev`
  - Open the URL printed by Vite (usually `http://localhost:5173`).

### Docs
- High-level design and detailed plan live in `docs/plan.md`.
- Remaining tasks are tracked in `docs/Todo.md`.

This project is still a work in progress; interfaces and behavior may change.