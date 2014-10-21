ACTION_IDLE = 1
ACTION_WALK = 2

DIRECTION_NORTH = 1
DIRECTION_EAST = 2
DIRECTION_SOUTH = 3
DIRECTION_WEST = 4

SPEED_WALK = 50

function DoActorIdle(direction)
    if actorAction == ACTION_IDLE and actorDirection == direction then
        return
    end

    local animations =
    {
        [DIRECTION_NORTH] = alias..".Idle.N",
        [DIRECTION_EAST] = alias..".Idle.E",
        [DIRECTION_SOUTH] = alias..".Idle.S",
        [DIRECTION_WEST] = alias..".Idle.W"
    }

    ActorPhysicsSetVelocity(actorId, {x = 0, y = 0})
    ActorAnimationSet(actorId, animations[direction])
    ActorAnimationPlay(actorId, false)

    actorDirection = direction
    actorAction = ACTION_IDLE
end

function DoActorWalk(direction)
    if actorAction == ACTION_WALK and actorDirection == direction then
        return
    end

    local animations =
    {
        [DIRECTION_NORTH] = alias..".Walk.N",
        [DIRECTION_EAST] = alias..".Walk.E",
        [DIRECTION_SOUTH] = alias..".Walk.S",
        [DIRECTION_WEST] = alias..".Walk.W"
    }

    local velocities =
    {
        [DIRECTION_NORTH] = {x = 0, y = -SPEED_WALK},
        [DIRECTION_EAST] = {x = SPEED_WALK, y = 0},
        [DIRECTION_SOUTH] = {x = 0, y = SPEED_WALK},
        [DIRECTION_WEST] = {x = -SPEED_WALK, y = 0}
    }

    ActorPhysicsSetVelocity(actorId, velocities[direction])
    ActorAnimationSet(actorId, animations[direction])
    ActorAnimationPlay(actorId, true)

    actorDirection = direction
    actorAction = ACTION_WALK
end

function OnActorCreate(id, parameters)
    actorId = id
    start = ActorGetPosition(actorId)
    alias = ActorGetAlias(actorId)
 	walkMinTime  = parameters.walkMinTime
    walkMaxTime  = parameters.walkMaxTime
    idleMinTime  = parameters.idleMinTime
    idleMaxTime  = parameters.idleMaxTime
    wanderDist   = parameters.wanderDist
    actionTimer = idleMinTime

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
    if actionTimer <= 0 then
        if actorAction == ACTION_IDLE then
            newDirection = math.random(1,4)
            if newDirection == actorDirection then
                newDirection = newDirection + 1
                if newDirection > 4 then
                    newDirection = 1
                end
            end
            actionTimer = math.random() * (walkMaxTime - walkMinTime) + walkMinTime
            DoActorWalk(newDirection)
        else
            actionTimer = math.random() * (idleMaxTime - idleMinTime) + idleMinTime
            DoActorIdle(actorDirection)
        end
        return
    end

    if actorAction == ACTION_WALK then
        newPos = ActorGetPosition(actorId)
        if newPos.y > (start.y + wanderDist) then
            DoActorWalk(DIRECTION_NORTH)
        elseif newPos.y < (start.y - wanderDist) then
            DoActorWalk(DIRECTION_SOUTH)
        elseif newPos.x > (start.x + wanderDist) then
            DoActorWalk(DIRECTION_WEST)
        elseif newPos.x < (start.x - wanderDist) then
            DoActorWalk(DIRECTION_EAST)
        end
    end
    actionTimer = actionTimer - elapsed
end

function OnActorReceiveMessage(message, parameters)
    if message == "interact" then
        actorPos = ActorGetPosition(actorId)
        playerPos = ActorGetPosition(parameters.user)

        if math.abs(playerPos.x - actorPos.x) > math.abs(playerPos.y - actorPos.y) then
            if playerPos.x > actorPos.x then
                DoActorIdle(DIRECTION_EAST)
            else
                DoActorIdle(DIRECTION_WEST)
            end
        else
            if playerPos.y < actorPos.y then
                DoActorIdle(DIRECTION_NORTH)
            else
                DoActorIdle(DIRECTION_SOUTH)
            end
        end
        actionTimer = 4
    end
end
