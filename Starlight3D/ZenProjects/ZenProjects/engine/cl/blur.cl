
int getLoc(int x,int y,int iw){

    return (y*iw*3)+(x*3);

}

void kernel blurImage(global unsigned char* inImg,global unsigned char * outImg,float blurFactor,int imgW,int imgH )
{
   
    int i = get_global_id(0);

    int sy = i / imgW;

    int sx = i - (sy*imgW);


    unsigned int cr,cg,cb;
    cr=0;
    cg=0;
    cb=0;

    float rx,ry;

    rx = sx;
    ry = sy;

    int cx,cy;

    cx=-8;
    cy=-8;
    int samples=0;

    while(true){

        int nx = rx + cx*3;
        int ny = ry + cy*3;

        if(nx<imgW && ny<imgH && nx>0 && ny>0){

            int aloc = (ny*imgW*3)+(nx*3);


            cr += inImg[aloc];
            cg += inImg[aloc+1];
            cb += inImg[aloc+2];

        }

     //   cr = cr + (unsigned int)inImg[aloc];

        cx=cx+1;
        if(cx==8){
            cx=-8;
            cy=cy+1;
            if(cy==8)
            {
                break;
            }
        }
        samples=samples+1;
    }

    cr = cr / samples;
    cg = cg / samples;
    cb = cb / samples;


    int loc = i*3;


    outImg[loc] = cr;
    outImg[loc+1] = cg;
    outImg[loc+2] = cb;

  

}