# Design Document: Fix Hero Movement

## Overview

This design addresses the hero movement bug by restructuring the `player_update` function in `input.c`. The core issue is that movement interpolation code is nested inside the "not moving" condition, preventing the hero from actually moving. The fix separates movement initiation from movement interpolation into two distinct code blocks.

## Architecture

The movement system follows a state machine pattern:
- **Idle State**: Hero is not moving, waiting for input
- **Moving State**: Hero is interpolating between tiles

The `player_update` function will be restructured to handle these states separately:

1. **Always update facing direction** when arrow keys are pressed
2. **Movement Initiation Block**: Only executes when `!moving` - checks walkability and sets target
3. **Movement Interpolation Block**: Only executes when `moving` - performs pixel interpolation

## Components and Interfaces

### Modified Function: `player_update(Game *game)`

**Location**: `input.c`

**Current Structure** (buggy):
```
1. Get keyboard state and direction
2. Update facing if direction pressed
3. IF not moving AND direction pressed:
   a. Check walkability
   b. Set moving = true and update target
   c. IF moving:  ← BUG: This never executes because we're inside "!moving"
      - Interpolate position
      - Check arrival
```

**New Structure** (fixed):
```
1. Get keyboard state and direction
2. Update facing if direction pressed
3. IF not moving AND direction pressed:
   a. Check walkability
   b. Set moving = true and update target
4. IF moving:  ← Moved outside, now executes independently
   a. Interpolate position
   b. Check arrival
   c. Update camera
   d. Check for exits
```

### Helper Functions (unchanged)

- `get_dir_from_key(const Uint8* keys)` - Converts keyboard state to direction
- `dir_to_offset(Direction dir, int* dx, int* dy)` - Converts direction to tile offset
- `map_update(Game *game)` - Handles map transitions

## Data Models

No changes to data structures. The fix only involves control flow restructuring.

**Relevant Fields**:
- `game->hero.moving` (bool) - Movement state flag
- `game->hero.tile_x, tile_y` (int) - Target tile coordinates
- `game->hero.px, py` (int) - Target pixel coordinates
- `game->hero.x, y` (float) - Current interpolated pixel coordinates
- `game->hero.speeding` (float) - Movement speed per frame

## Correctness Properties

*A property is a characteristic or behavior that should hold true across all valid executions of a system—essentially, a formal statement about what the system should do. Properties serve as the bridge between human-readable specifications and machine-verifiable correctness guarantees.*

### Property 1: Movement State Transition
*For any* game state where the hero is not moving and a valid direction is pressed, if the target tile is walkable, then the hero's moving state should become true and target positions should be updated.

**Validates: Requirements 1.1, 1.2**

### Property 2: Interpolation Progress
*For any* game state where the hero is moving, the hero's current position should move closer to the target position by the speed amount each frame.

**Validates: Requirements 2.1**

### Property 3: Movement Completion
*For any* game state where the hero is moving and the distance to target is less than the speed threshold, the moving state should become false and the position should be locked to the target.

**Validates: Requirements 2.2**

### Property 4: Facing Direction Independence
*For any* game state and any direction input, the hero's facing direction should update regardless of the moving state.

**Validates: Requirements 1.3**

### Property 5: Movement Exclusivity
*For any* game state where the hero is moving, new movement commands should not change the target tile until the current movement completes.

**Validates: Requirements 3.4**

## Error Handling

The fix does not introduce new error conditions. Existing error handling remains:
- Boundary checks for tile coordinates (implicit in walkability check)
- Camera boundary clamping (existing code)
- Map transition validation (existing code)

## Testing Strategy

### Unit Tests

Since this is a bug fix in existing code, testing will focus on:

1. **Movement Initiation Test**: Verify that pressing arrow keys when not moving sets the moving flag and updates target positions
2. **Interpolation Test**: Verify that when moving flag is true, position interpolates toward target
3. **Movement Completion Test**: Verify that moving flag becomes false when target is reached
4. **Facing Update Test**: Verify facing direction updates regardless of moving state
5. **Blocked Movement Test**: Verify that unwalkable tiles prevent movement initiation

### Property-Based Tests

For this bug fix, property-based testing is not practical because:
- The code interacts heavily with SDL2 state (keyboard, rendering)
- Movement is frame-dependent and stateful
- Testing would require mocking the entire SDL2 environment

Instead, manual testing will verify:
- Hero moves smoothly in all four directions
- Hero cannot walk through walls
- Hero stops at tile boundaries
- Camera follows hero correctly
- Map transitions work at exit tiles

### Testing Configuration

- **Framework**: Manual testing with the compiled game
- **Test Cases**: Move in all 4 directions, test wall collision, test map transitions
- **Success Criteria**: Hero moves smoothly between tiles without getting stuck
