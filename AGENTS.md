# Project

Laundry Machine Kiosk

## Hardware:
* Raspberry Pi 4B
* LCD Touch TFT 7" (800x480)

## Framework:
* Qt6 Widgets
* CMake

## Goals:
* Machine selection screen
* Payment flow
* Machine status monitoring
* Admin screen

## Documentation:
* docs/Requirement.md
* docs/UI_Spec.md
* docs/Screens/LaundryKioskFlow.html

## Build Command:
Run the following command to build the project:
```
./scripts/build.sh
```

## Investigation Guidelines:
To investigate the project, follow this order:  
1. Read `README*`, root manifests, and workspace configurations first.
2. Check the configurations for build, test, lint, formatter, typecheck, and codegen.
3. Inspect CI workflows and pre-commit/task runner configurations.
4. Review existing instruction files such as `AGENTS.md` and `.github/copilot-instructions.md`.
5. Evaluate the local OpenCode configuration in `opencode.json`.
6. If necessary, read small, representative code files to understand entry points and package boundaries.

## Commands:
### Running Tests:
To run tests, use the command:
```
<insert test command>
```

### Specific Verification Steps:
* Always perform the following in this order when making changes:  
  - `lint` → `typecheck` → `test`

## Package Boundaries:
* Note the ownership of major directories; categorize them as app or library entry points based on the structure.

## Framework Quirks:
* Be aware that any generated code or migrations must be handled carefully. Special attention should be paid to build artifacts and environment loading processes.

## Testing Quirks:
* Confirm any fixtures or integration test prerequisites before running tests. Some tests may be flaky or require additional services to be running to succeed.

## Important Constraints:
* Ensure that the UI is always optimized for the 800x480 resolution as stated in the project rules.
* Update any documented team conventions or branch expectations here when known.

## Notes:
* Include references to existing instructions where necessary for context.