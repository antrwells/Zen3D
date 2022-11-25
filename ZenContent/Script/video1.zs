class video1 > Node3D

    string videoPath

    CObj video
    CObj vimg

    method void Init()


        video = Video.StartVideo(videoPath)

    end 

    method void Update()
 vimg = Video.NextFrame(video)
    end 

    method void UI()

       

        GameUI.DrawTexture(vimg,0,0,GameUI.DisplayWidth(),GameUI.DisplayHeight(),1,1,1,1)

    end 

    method void Render()

    end 

end 