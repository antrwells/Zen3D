class RayHit

    int Hit = 0
    Vec3 HitPosition = new Vec3(0,0,0)
    Vec3 HitOrigin = new Vec3(0,0,0)
    float HitDistance = 0
    Node3D HitNode
    
end 

static class GameScene

    method RayHit Raycast(Vec3 pos,Vec3 dst,Node ignore)
        
        RayHit res
        
        res = GameSceneRayCast(pos,dst,ignore)

        printf("Hit?",res.Hit)
        printf("Dist:",res.HitDistance)



    end      


end 