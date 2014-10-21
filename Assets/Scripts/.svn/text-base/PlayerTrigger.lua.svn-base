function OnActorCreate(id, parameters)
    triggerId = id
    triggerWidth = parameters.width
    triggerHeight = parameters.height
    triggerPersist = parameters.persist
    triggerCode = parameters.code

    ActorSetShape(
        triggerId,
        ACTOR_SHAPE_TYPE_COLLISION_INTERACT,
        { size = {x = triggerWidth, y = triggerHeight }, position = { x = 0, y = 0 }, data = 1, enabled = true}
    )
end

function OnActorUpdate(elapsed)
    actorIds = ActorPhysicsQueryShapeCollisions(triggerId, ACTOR_SHAPE_TYPE_COLLISION_INTERACT)

    for index, value in ipairs(actorIds) do
        if ActorGetName(value) == "player" then
--            ActorScriptDoString(triggerId, triggerCode)
            if not triggerPersist then
                ActorDisableShape(triggerId, ACTOR_SHAPE_TYPE_COLLISION_INTERACT)
            end
        end
    end
end
