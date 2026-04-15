# Requirements Document

## Introduction

This document specifies the requirements for fixing the hero movement bug in the SDL2 tile-based game. The hero currently cannot move because the movement interpolation logic is incorrectly nested within the movement initiation condition.

## Glossary

- **Hero**: The player-controlled character in the game
- **Tile**: A 16x16 pixel grid cell that defines walkable/non-walkable areas
- **Movement_System**: The subsystem responsible for hero movement and interpolation
- **Interpolation**: Smooth pixel-by-pixel movement between tile positions
- **Tile_Position**: The discrete grid coordinates (tile_x, tile_y)
- **Pixel_Position**: The continuous screen coordinates (x, y)

## Requirements

### Requirement 1: Hero Movement Initiation

**User Story:** As a player, I want to press arrow keys to start moving the hero, so that I can navigate the game world.

#### Acceptance Criteria

1. WHEN a player presses an arrow key AND the hero is not currently moving, THE Movement_System SHALL check if the target tile is walkable
2. IF the target tile is walkable, THEN THE Movement_System SHALL set the hero's moving state to true and update the target tile position
3. WHEN a player presses an arrow key, THE Movement_System SHALL update the hero's facing direction regardless of movement state

### Requirement 2: Hero Movement Interpolation

**User Story:** As a player, I want the hero to move smoothly between tiles, so that the movement feels natural and not jerky.

#### Acceptance Criteria

1. WHILE the hero is in moving state, THE Movement_System SHALL interpolate the hero's pixel position toward the target pixel position
2. WHEN the hero's pixel position reaches the target position (within speed threshold), THE Movement_System SHALL set moving state to false
3. THE Movement_System SHALL update the camera position to keep the hero centered during movement
4. WHEN interpolation completes, THE Movement_System SHALL check for exit tiles and trigger map transitions if needed

### Requirement 3: Movement State Management

**User Story:** As a developer, I want clear separation between movement initiation and interpolation, so that the code is maintainable and bug-free.

#### Acceptance Criteria

1. THE Movement_System SHALL separate movement initiation logic from interpolation logic
2. WHEN the hero is not moving, THE Movement_System SHALL only process new movement commands
3. WHEN the hero is moving, THE Movement_System SHALL only process interpolation until completion
4. THE Movement_System SHALL prevent new movement commands while interpolation is in progress
