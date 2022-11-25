class Node3D

    CObj (Node) 

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

    method void SetPhysicsToBox(float size)

        Node3DSetPyToBox(Node,size)

    end

    method void SetPhysicsToTriMesh()

        Node3DSetPyTriMesh(Node)

    end 

    method void SetPhysicsToConvex()

        Node3DSetPyToConvex(Node)

    end 

    method void SetPhysicsToCylinder(float height,float width)

        Node3DSetPyToCylinder(Node,height,width)

    end 

    method string GetName()

        return Node3DGetName(Node)

    end 

    method int IsNode(Node3D node)

        if Node == node.Node

            return 1

        end
        
        return 0

    end 

end 