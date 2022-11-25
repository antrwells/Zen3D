class RayHit

    int Hit = 0
    Vec3 HitPosition = new Vec3(0,0,0)
    Vec3 HitOrigin = new Vec3(0,0,0)
    float HitDistance = 0
    Node3D HitNode
    
end 

static class GameScene


    method void LoadScene(string path)

        GameSceneLoadScene(path)

    end 

    method Node3D GetSceneNode(string name)

        Node3D res = GameSceneGetNode(name)

        return res


    end 

    method RayHit Raycast(Vec3 pos,Vec3 dst,Node ignore)
        
        RayHit res
        
        res = GameSceneRayCast(pos,dst,ignore)

        return res

    end      


end 