static class GameProperties

     method void SetProperty(string name,var val)

      SystemSetProperty(name,val)

    end 

    method var GetProperty(string name)

        return SystemGetProperty(name)

    end 

end 