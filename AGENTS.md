# AGENTS.md for QuanLyMayGiat Repository

## Investigation Guidelines
- **Read Highest-Value Sources First**:
  - `README*`, root manifests, workspace config, lockfiles.
  - Check all configurations related to build, test, lint, formatter, typecheck, and codegen.
  - Review CI workflows and pre-commit/task runner configurations.
  - Look into existing instruction files (`AGENTS.md`, `CLAUDE.md`, etc.).
  - Inspect local OpenCode configurations (e.g., `opencode.json`).

- **Executable Sources of Truth**: Trust configurations and code over documentation. Focus on files that clearly explain how the system works together rather than isolated leaf files.

## Important Commands
- **Running Tests**:
  - To run all tests: `make test`
  - To run a single test: `./build/test_name`

- **Build and Run**:
  - Build the project: `cmake .. && make`
  - Set up environment variables to run the application or tests correctly as needed.

## Architecture Notes
- The project operates in a **monorepo** setup with distinct packages. The file structure clearly separates logical components:
  - `src`: Contains source code.
  - `tests`: Contains testing code.

- Main entry points are in the `src` subdirectories, where each driver or component has its implementations.

## Workflow Quirks
- Ensure proper loading of environmental variables via `.env` files; refer to configurations for any special requirements.
- Be aware of special build artifacts and generated code that may exist due to the specific frameworks used.

## Testing Notes
- Integration tests may have prerequisites; ensure all dependent services are up before running tests.
- Watch for flaky tests that might require special handling or additional setup.

---
Keep this file up-to-date with additional notes as they become evident from development progress in the repo.