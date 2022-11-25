static class GameUI

    method int Button(string text,int x,int y,int w,int h)

        return GUIButton(text,x,y,w,h)

    end 

    method int Image(Image2d img,int x,int y,int w,int h,float r,float g,float b,float a)

        GUIImage(img,x,y,w,h,r,g,b,a)

    end 

    method void DrawTexture(CObj tex,int x,int y,int w,int h,float r,float g,float b,float a)

        UtilDrawTexture(tex,x,y,w,h,r,g,b,a)

    end 

    method int DisplayWidth()

        return GUIDisplayWidth()

    end

    method int DisplayHeight()

        return GUIDisplayHeight()

    end

end 

class Image2D

    CObj Image

    method void Image2D(string file)

        Image = GUILoadImage(file)

    end 

end 