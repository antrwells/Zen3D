class Node3D

    CObj Node

    method void Node3D()

      

    end 

    method void check1()

        printf("Check1 called")

    end 

    method void Turn(float pitch,float yaw,float roll)

        Node3DTurn(Node,pitch,yaw,roll)

    end 

    method void SetRotation(float pitch,float yaw,float roll)

        Node3DSetRotation(Node,pitch,yaw,roll)    
    
    end 

    method Vec3 GetPosition()

        return Node3DGetPos(Node)

    end 

    method void SetPosition(Vec3 pos)

        Node3DSetPos(Node,pos)

    end 

 

end 