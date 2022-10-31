static class Video

    method CObj StartVideo(string path)

        return VideoStart(path)

    end 

    method CObj NextFrame(CObj video)

        return VideoNextFrame(video)

    end 

end 