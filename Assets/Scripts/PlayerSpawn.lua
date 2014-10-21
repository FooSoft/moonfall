function OnActorCreate(id, parameters)
    spawnId = id
    spawnPlayerDirection = parameters.playerDirection
    spawnPlayerAlias = parameters.playerAlias
    spawnPersist = parameters.persist

    spawnPosition = ActorGetPosition(spawnId)
    spawnPosition.x = spawnPosition.x
    spawnPosition.y = spawnPosition.y - 24

    ActorSetShape(
        spawnId,
        ACTOR_SHAPE_TYPE_COLLISION_INTERACT,
        { size = {x = 32, y = 32 }, position = { x = 0, y = 0 }, data = 0, enabled = true}
    )
end

function OnActorReceiveMessage(message, parameters)
    if message ~= "spawn" then
        return
    end

    playerId = ActorAdd(spawnPlayerAlias, spawnPosition, "player", { direction = spawnPlayerDirection })
    playerShape = ActorGetShape(playerId, ACTOR_SHAPE_TYPE_COLLISION_SOLID)

    if not spawnPersist then
        ActorRemove(spawnId)
    end

    return { playerId = playerId }
end
