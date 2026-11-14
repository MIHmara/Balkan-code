# Maze – Project Plan (Scrum)

## 1. Timeline

| Period | Event |
|---|---|
| 21.10.2026 | Start, Sprint 1 planning |
| 21.10 – 30.10 | Sprint 1 – foundation |
| 31.10 – 07.11 | Sprint 2 – full game |
| 08.11 – 14.11 | Sprint 3 – quality, documentation, presentation |
| 15.11.2026 | Deadline – final release on GitHub |
| 16–17.11 | Presentation and defence |

Every sprint ends with a short review (what works) and a retrospective (what to keep,
what to change).

## 2. Roles

| Role | Responsibilities |
|---|---|
| Scrum Trainer | Backlog and board in GitHub Projects, sprint meetings, repository rules, `GameConfig.h`, build files, README, presentation |
| Backend Developer | `Maze`, `Game`, `Validation` – data structures, algorithms, rules |
| Frontend Developer | `Input`, `Renderer`, `main` – menu, keyboard, drawing |
| QA | Test framework, automated tests, manual testing, bug reports, QA documentation |

Each member writes the technical description of their own part.

## 3. Product backlog

| ID | User story | Owner | Sprint |
|---|---|---|---|
| US-01 | As a player I want to choose the maze size (10–50) so I can choose the difficulty | Frontend + Backend | 1 |
| US-02 | As a player I want a new random maze every game | Backend | 1 |
| US-03 | As a player I want to see the maze and my position after every move | Frontend | 1 |
| US-04 | As a player I want to move with the arrow keys | Frontend | 1 |
| US-05 | As a player I want walls to stop me | Backend | 1 |
| US-06 | As a player I want to win when I reach the exit | Backend | 2 |
| US-07 | As a player I want a menu with help and about screens | Frontend | 2 |
| US-08 | As a player I want clear error messages for wrong input | Backend + Frontend | 2 |
| US-09 | As a player I want keys and special doors that shorten the way | Backend | 2 |
| US-10 | As a player I want inventory and location information | Frontend | 2 |
| US-11 | As a player I want a rating and a seed to replay a maze | Backend + Frontend | 3 |
| US-12 | As a team we want automated tests for the logic | QA | 1–3 |
| US-13 | As a team we want documentation and a presentation | All | 3 |

**Definition of Done:** the code compiles without errors and warnings, follows the
naming conventions, has meaningful comments, is reviewed in a pull request, the tests
pass, and the documentation is updated.

## 4. GitHub

### 4.1 Repository

- One shared repository; every member has access (Settings → Collaborators).
- The teacher is added as a collaborator or the repository is public.
- Folder structure: `include/`, `src/`, `tests/`, `docs/` (see README).

### 4.2 Board (GitHub Projects)

Columns: **Backlog → To Do → In Progress → Review → Done**. Every user story and task is
an Issue with an owner, a sprint milestone and a label (`feature`, `bug`, `docs`, `test`).

### 4.3 Branches

- `main` – always working code
- `feature/<short-name>` – one branch per task, e.g. `feature/maze-generation`,
  `feature/arrow-keys`, `test/game-rules`, `docs/qa`
- Merge into `main` only through a pull request reviewed by another member

### 4.4 Commit messages

Format: `<type>: <short summary>` in English, imperative mood, up to 72 characters,
followed by an optional description.

| Type | Use for | Example |
|---|---|---|
| `feat` | New functionality | `feat: add special doors that shorten the route` |
| `fix` | Bug fix | `fix: prevent moving outside the maze on the left edge` |
| `test` | Tests | `test: add boundary tests for maze size validation` |
| `docs` | Documentation | `docs: describe BFS in technical documentation` |
| `refactor` | Code change without new behaviour | `refactor: move drawing functions to Renderer` |
| `build` | Build files | `build: add CMakeLists with test target` |

Example with description:

```
feat: generate maze with randomized depth-first search

Rooms are tiles with even coordinates. A dynamic stack replaces
recursion so 50x50 mazes cannot overflow the call stack.
Closes #4
```

## 5. Sprint tasks (suggested)

| Sprint | Backend | Frontend | QA | Scrum Trainer |
|---|---|---|---|---|
| 1 | `Maze` structure, create/destroy, DFS generation, `Validation` | `Input` (arrows, numbers), `drawMaze`, basic loop | Test framework, TC-V, TC-M01–M08 | Repository, board, backlog, build files |
| 2 | `movePlayer`, exit, doors, keys, BFS | Menu, help, inventory, location screens | TC-M09–M13, TC-G01–G08, manual tests | Sprint review, README |
| 3 | Rating, par, seed | Victory/quit screens, polish | TC-G09–G12, test results, bug fixes check | Technical doc assembly, presentation, release |
