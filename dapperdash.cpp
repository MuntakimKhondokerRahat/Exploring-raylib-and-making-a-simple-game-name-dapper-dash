#include "raylib.h"

struct Animdata
{
  Rectangle rec;
  Vector2 pos;
  int frame;
  float updatetime;
  float runningtime;
};

bool isonground(Animdata data, int windowHeight)
{
  return data.pos.y>=windowHeight-data.rec.height;
}
Animdata updateAnimdata(Animdata data ,float deltatime,int maxframe)
{
  data.runningtime  += deltatime;
  if(data.runningtime >= data.updatetime)
  {
    data.runningtime = 0.0;
    data.rec.x = data.frame * data.rec.width;
    data.frame++;
    if(data.frame > maxframe)
    {
      data.frame = 0;
    }
  }
  return data;
}
int main()
{
  int windowDimensions[2];
  windowDimensions[0]=512;
  windowDimensions[1]=380;
  InitWindow(windowDimensions[0],windowDimensions[1],"Dapper Dasher!");
  
  
    // accleration due to gravity(pixels/s)/s
    const int gravity{1'000};
    //nebula variable
    Texture2D nebula =LoadTexture("textures/12_nebula_spritesheet.png");
    const int sizeofnebulae{5};
    Animdata nebulae[sizeofnebulae]{};
    for (int i = 0; i < sizeofnebulae; i++)
    {
      nebulae[i].rec.x=0.0;
      nebulae[i].rec.y=0.0;
      nebulae[i].rec.width= nebula.width/8;
      nebulae[i].rec.height = nebula.height/8;
      nebulae[i].pos.y=windowDimensions[1]-nebula.height/8;
      nebulae[i].frame=0;
      nebulae[i].runningtime = 0.0;
      nebulae[i].updatetime= 1.0/16.0;
      nebulae[i].pos.x=windowDimensions[0]+i*300;

    }
    float finishline{nebulae[sizeofnebulae-1].pos.x};
    

    //nebula x velocity
    int nebvel{-200};

   //scarfy variable
    Texture2D scarfy=LoadTexture("textures/scarfy.png");
    Animdata scarfydata;
    scarfydata.rec.width=scarfy.width/6;
    scarfydata.rec.height=scarfy.height;
    scarfydata.rec.x=0;
    scarfydata.rec.y=0;
    scarfydata.pos.x=windowDimensions[0]/2-scarfydata.rec.width/2;
    scarfydata.pos.y=windowDimensions[1]-scarfydata.rec.height;
    scarfydata.frame=0;
    scarfydata.updatetime=1.0/12.0;
    scarfydata.runningtime=0.0;
 
    //is rectangle in air
    bool isInAir{};
    //jum velocity pixel/s
    const int jumpVel{-600};

    int velocity{0};
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgx{};
    Texture2D midground = LoadTexture("texture/back-buildings.png");
    float mgx{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgx{};

  SetTargetFPS(60);
     bool collision {};
    while(!WindowShouldClose())
    {
      //delta time(time since last time)
       const float dT{GetFrameTime()};

       BeginDrawing();
       ClearBackground(WHITE);

      bgx -= 20*dT;
      if(bgx <= -background.width*2)
      {
        bgx=0;
      }

      mgx -= 40 * dT;
      if(mgx <= -midground.width * 2)
      {
        mgx = 0.0;
      }

      fgx -= 80 *dT;
      if(fgx <= -foreground.width*2)
      {
        fgx = 0.0;
      }

       //drawbackground
       Vector2 bg1pos{bgx,0.0};
       DrawTextureEx(background,bg1pos,0.0,2.0,WHITE);
       Vector2 bg2pos{bgx+background.width*2,0.0};
       DrawTextureEx(background,bg2pos,0.0,2.0,WHITE);

       Vector2 mg1pos{mgx,0.0};
       DrawTextureEx(midground,mg1pos, 0.0, 2.0,WHITE);
       Vector2 mg2pos{mgx+midground.width*2,0.0};
       DrawTextureEx(midground,mg2pos,0.0,2.0,WHITE);

       Vector2 fg1pos{fgx,0.0};
       DrawTextureEx(foreground,fg1pos, 0.0, 2.0,WHITE);
       Vector2 fg2pos{fgx+foreground.width*2,0.0};
       DrawTextureEx(foreground,fg2pos,0.0,2.0,WHITE);

      

       //perform ground check
       if(isonground(scarfydata,windowDimensions[1]))
          {
            //rectangle is on the ground
            velocity=0;
            isInAir=false;
          }
          else
          {
            //rectangle is on the air
              velocity+=gravity*dT;
              isInAir=true;
          }

       //jump check
       if(IsKeyPressed(KEY_SPACE) && !isInAir)
       {
        velocity+=jumpVel;
       }
       
       for (int i = 0; i < sizeofnebulae; i++)
       {
        nebulae[i].pos.x+=nebvel*dT;
       }
       finishline += nebvel * dT;
  
       

       //update scarfy position
       scarfydata.pos.y+=velocity*dT;

       //update scarfy animation frame
        if(!isInAir)
          {
           scarfydata = updateAnimdata(scarfydata,dT,5);
        }


       for (int i = 0; i < sizeofnebulae; i++)
       {
          nebulae[i] = updateAnimdata(nebulae[i],dT,7);

       }
        
    
       for (Animdata nebula : nebulae)
       {
        float pad{50};
         Rectangle nebRec{
          nebula.pos.x + pad,
          nebula.pos.y + pad,
          nebula.rec.width - 2 * pad,
          nebula.rec.height - 2 * pad,
         };
         Rectangle scarfyRec{
          scarfydata.pos.x,
          scarfydata.pos.y,
          scarfydata.rec.width,
          scarfydata.rec.height
         };
         if(CheckCollisionRecs(nebRec,scarfyRec))
         {
          collision=true;
         }
       }
       if(collision)
       {
          DrawText("GAME OVER! ",windowDimensions[0]/4,windowDimensions[1]/2,40,RED);
       }
       else if ( scarfydata.pos.x >=finishline)
       {
         DrawText("YOU WIN ",windowDimensions[0]/4,windowDimensions[1]/2,40,RED);
       }
       else
       {
            for (int i = 0; i < sizeofnebulae; i++)
            {
              //draw nebula
            DrawTextureRec(nebula,nebulae[i].rec,nebulae[i].pos,WHITE);
            }
          DrawTextureRec(scarfy,scarfydata.rec,scarfydata.pos,WHITE);
          
      }
       EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}
