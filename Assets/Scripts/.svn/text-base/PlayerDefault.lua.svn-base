ACTION_IDLE = 1
ACTION_WALK = 2

DIRECTION_NORTH = 1
DIRECTION_EAST = 2
DIRECTION_SOUTH = 3
DIRECTION_WEST = 4

function DoActorIdle(direction)
    if playerAction == ACTION_IDLE and playerDirection == direction then
        return
    end

    ActorPhysicsSetVelocity(playerId, {x = 0, y = 0})
    ActorAnimationSet(playerId, playerIdleAnimations[direction])
    ActorAnimationPlay(playerId, true)

    playerDirection = direction
    playerAction = ACTION_IDLE
end

function DoActorWalk(direction)
    if playerAction == ACTION_WALK and playerDirection == direction then
        return
    end

    ActorPhysicsSetVelocity(playerId, playerWalkVelocities[direction])
    ActorAnimationSet(playerId, playerWalkAnimations[direction])
    ActorAnimationPlay(playerId, true)

    playerDirection = direction
    playerAction = ACTION_WALK
end

function DoActorUse()
    actorsIds = ActorPhysicsQueryShapeCollisions(playerId, ACTOR_SHAPE_TYPE_COLLISION_INTERACT)
    for i, v in ipairs(actorsIds) do
        ActorScriptSendMessage(v, "interact", { user = playerId })
    end
end

function OnActorCreate(id, parameters)
    playerId = id

    local actorAlias = ActorGetAlias(playerId)
    local speedWalk = 125

    playerWalkVelocities =
    {
        [DIRECTION_NORTH] = {x = 0, y = -speedWalk},
        [DIRECTION_EAST] = {x = speedWalk, y = 0},
        [DIRECTION_SOUTH] = {x = 0, y = speedWalk},
        [DIRECTION_WEST] = {x = -speedWalk, y = 0}
    }

    playerWalkAnimations =
    {
        [DIRECTION_NORTH] = actorAlias .. ".Walk.N",
        [DIRECTION_EAST] = actorAlias .. ".Walk.E",
        [DIRECTION_SOUTH] = actorAlias .. ".Walk.S",
        [DIRECTION_WEST] = actorAlias .. ".Walk.W"
    }

    playerIdleAnimations =
    {
        [DIRECTION_NORTH] = actorAlias .. ".Idle.N",
        [DIRECTION_EAST] = actorAlias .. ".Idle.E",
        [DIRECTION_SOUTH] = actorAlias .. ".Idle.S",
        [DIRECTION_WEST] = actorAlias .. ".Idle.W",
    }

    local directionValues =
    {
        ["north"] = DIRECTION_NORTH,
        ["south"] = DIRECTION_SOUTH,
        ["west"] = DIRECTION_WEST,
        ["east"] = DIRECTION_EAST
    }

    local direction = directionValues[parameters.direction]
    assert(direction ~= nil)

    DoActorIdle(direction)
end

function OnActorUpdate(elapsed)
    if InputIsKeyTriggered(META_KEY_USE) then
        DoActorUse()
        return
    elseif InputIsKeyPressed(META_KEY_UP) then
        DoActorWalk(DIRECTION_NORTH)
        return
    elseif InputIsKeyPressed(META_KEY_DOWN) then
        DoActorWalk(DIRECTION_SOUTH)
        return
    elseif InputIsKeyPressed(META_KEY_LEFT) then
        DoActorWalk(DIRECTION_WEST)
        return
    elseif InputIsKeyPressed(META_KEY_RIGHT) then
        DoActorWalk(DIRECTION_EAST)
        return
    end

    DoActorIdle(playerDirection)
end
