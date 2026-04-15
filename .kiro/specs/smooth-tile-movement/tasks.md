# Implementation Plan: Smooth Tile Movement

## Overview

This plan implements Pokemon-style smooth tile movement using frame-based linear interpolation. The implementation replaces pixel-speed movement with fixed-frame-duration movement for consistent, smooth animation.

## Tasks

- [x] 1. Update game.h with new constants and hero structure fields
  - Add `#define MOVE_FRAMES 8` constant for movement duration
  - Add `float start_x, start_y` fields to SB structure for interpolation start position
  - Add `int move_frames` field to SB structure for total frames per movement
  - Add `int move_frame_count` field to SB structure for current frame counter
  - Remove `float speeding` field (no longer needed)
  - _Requirements: 1.1, 1.2, 4.1, 4.2, 4.3_

- [x] 2. Update Game_Init in game.c to initialize new hero fields
  - Set `game->hero.move_frames = MOVE_FRAMES`
  - Set `game->hero.move_frame_count = 0`
  - Set `game->hero.start_x = game->hero.x`
  - Set `game->hero.start_y = game->hero.y`
  - Remove `game->hero.speeding = 2.0f` initialization
  - _Requirements: 1.4, 4.4_

- [x] 3. Rewrite movement initiation logic in player_update (input.c)
  - When starting movement, store current position in `start_x` and `start_y`
  - Reset `move_frame_count = 0` when starting new movement
  - Keep existing walkability check and target tile update logic
  - _Requirements: 1.1, 1.4, 2.1_

- [x] 4. Rewrite movement interpolation logic in player_update (input.c)
  - Increment `move_frame_count` each frame during movement
  - Calculate progress ratio: `progress = (float)move_frame_count / (float)move_frames`
  - Apply linear interpolation: `hero.x = start_x + (px - start_x) * progress`
  - Apply linear interpolation: `hero.y = start_y + (py - start_y) * progress`
  - Replace distance-based completion check with frame-based: `if (move_frame_count >= move_frames)`
  - _Requirements: 2.1, 2.2, 2.3, 2.4_

- [x] 5. Implement per-frame camera updates in player_update (input.c)
  - Move camera update logic inside the movement interpolation block
  - Update camera position every frame based on current `hero.x` and `hero.y`
  - Update `hero_screen` position every frame for smooth rendering
  - Keep camera boundary clamping logic
  - Only check exit tiles and set `moving = false` when movement completes
  - _Requirements: 3.1, 3.2, 3.3, 3.4_

- [x] 6. Add exact position snapping at movement completion
  - When `move_frame_count >= move_frames`, snap position: `hero.x = px; hero.y = py`
  - Ensure no floating point drift accumulates over multiple movements
  - _Requirements: 2.4_

## Notes

- Default MOVE_FRAMES = 8 provides Pokemon-like movement speed at 60 FPS (~133ms per tile)
- Linear interpolation ensures smooth, consistent movement
- Per-frame camera updates eliminate screen jumping
- User will test different MOVE_FRAMES values to find optimal feel (6-12 recommended range)
