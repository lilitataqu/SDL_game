# Implementation Plan: Fix Hero Movement

## Overview

This plan fixes the hero movement bug by restructuring the `player_update` function to separate movement initiation from movement interpolation. The fix involves moving the interpolation code block outside of the "not moving" condition.

## Tasks

- [x] 1. Restructure player_update function in input.c
  - Extract the movement interpolation block (the nested `if (game->hero.moving)` block) from inside the `if (!game->hero.moving && dir != DIR_NONE)` condition
  - Place the interpolation block as a separate top-level condition after the movement initiation block
  - Ensure the interpolation block executes independently when `game->hero.moving` is true
  - Preserve all existing logic for position interpolation, camera updates, and exit tile checking
  - _Requirements: 1.1, 1.2, 2.1, 2.2, 3.1, 3.2, 3.3, 3.4_

- [x] 2. Verify code structure and logic flow
  - Review the restructured code to ensure movement initiation and interpolation are properly separated
  - Confirm that facing direction updates happen before movement checks
  - Verify that the moving flag correctly gates new movement commands
  - _Requirements: 1.3, 3.1, 3.4_

## Notes

- This is a pure refactoring task - no new functionality is added
- The fix only changes control flow structure, not the logic itself
- All existing movement, camera, and map transition logic remains unchanged
- User will perform manual testing after implementation
