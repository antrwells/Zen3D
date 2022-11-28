class propTest > Node3D

    method void Init()

       Vec3 v1

       v1 = new Vec3(22.0,30.0,5.0)

       GameProperties.SetProperty("Name",v1)
        
    end

    method void Update()
  
       var res 

       res = GameProperties.GetProperty("Name")

       printf("Testing:",res.x,res.y,res.z)

    end

    method void UI()

    end

    method void Render()

    end 

end 