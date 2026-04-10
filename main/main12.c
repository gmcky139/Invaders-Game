#include "task12-1.h"

char data_file[] = "task12-1.dat";

int main() {
    Display *d;
    Window w;
    XEvent e;
    int s;
    Atom wmDeleteMessage;
    int win_width = 640, win_height = 820;

    srand((unsigned int)time(NULL));


    d = XOpenDisplay(NULL);
    if (d == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, win_width, win_height, 1, BlackPixel(d, s), BlackPixel(d, s));
    XStoreName(d, w, "Invaders Game");
    wmDeleteMessage = XInternAtom(d, "WM_DELETE_WINDOW", False);

    XSetWMProtocols(d, w, &wmDeleteMessage, 1);

    //フォントの定義
    XFontStruct *fontBig;
    XFontStruct *fontSmall;
    fontBig = XLoadQueryFont(d, "12x24"); 
    fontSmall = XLoadQueryFont(d, "8x16");
    if (fontBig == NULL) {
        printf("フォントが見つかりません。デフォルトを使います。\n");
        fontBig = XLoadQueryFont(d, "fixed"); 
    }
    XSetFont(d, DefaultGC(d, s), fontBig->fid);


    // キー入力だけを受け取る設定
    XSelectInput(d, w, KeyPressMask | StructureNotifyMask | ButtonPressMask);
    XMapWindow(d, w);

    


    while (1) {
       XNextEvent(d, &e);
       if (e.type == MapNotify) break;
    }

    //リストの準備
    Bullet *bulletHead = NULL;
    Enemy *enemyHead = NULL;
    Supply *supplyHead = NULL;

    
    //タイトル画面まわり
    TITLE:
    //ハイスコアの読み出し
    int highScore = 0;
    FILE *fp;
    if((fp = fopen(data_file, "r")) == NULL){
        fp = fopen(data_file, "w");
        highScore = 0;
    }
    else{
        fp = fopen(data_file, "r");
        fscanf(fp, "%d", &highScore);
    }
    fclose(fp);

    char str1[] = "Invaders Game";
    char str2[] = "New Game";
    char str3[] = "Quit";
    char str4[30];
    sprintf(str4, "High Score: %010d", highScore);
    while (1){
        if (XPending(d) > 0) {
            XNextEvent(d, &e);

            if(e.type == ButtonPress){
                int x = e.xbutton.x;
                int y = e.xbutton.y;

                if(x > 245 && x < 395){
                    if(y > 500 && y < 550){
                        break;
                    }
                    else if(y > 600 && y < 650){
                        goto END_GAME;
                    }
                }
            }


            if(e.type == ClientMessage){
                if ((Atom)e.xclient.data.l[0] == wmDeleteMessage) {
                    goto END_GAME;
                }
            }
        }

       
        XClearWindow(d, w);
        XSetForeground(d, DefaultGC(d, s), WhitePixel(d, s));

        XDrawString(d, w, DefaultGC(d, s), 240, 200, str1, strlen(str1));
        XDrawString(d, w, DefaultGC(d, s), 190, 338, str4, strlen(str4));
        XDrawString(d, w, DefaultGC(d, s), 272, 538, str2, strlen(str2));
        XDrawString(d, w, DefaultGC(d, s), 298, 638, str3, strlen(str3));
        XDrawRectangle(d, w, DefaultGC(d, s), 245, 500, 150, 50);
        XDrawRectangle(d, w, DefaultGC(d, s), 245, 600, 150, 50);


        XFlush(d);
        usleep(30000);
    }

    //プレイヤーの定義
    struct Player player;
    InitializePlayer(&player);

    int score = 0;

    //難易度関連
    int elapsedTime = 0;
    int difficulty = 0;


    //本ループ
    while (1) {
        //入力処理
        if (XPending(d) > 0){
            XNextEvent(d, &e);
            if(e.type == KeyPress){
                KeySym key = XLookupKeysym(&e.xkey, 0);

                if(key == XK_a){
                    player.x -= player.speed;
                }
                else if(key == XK_d){
                    player.x += player.speed;
                }

                if(player.x< 40){
                    player.x = 40;
                }
                else if(player.x > 600){
                    player.x = 600;
                }
            }

            if(!player.inInterval && player.bulletsNum > 0 && e.type == ButtonPress){
                Bullet* newBullet = (Bullet*)malloc(sizeof(Bullet));

                if(newBullet != NULL){
                    newBullet -> x = player.x - 4;
                    newBullet -> y = player.y - 40;
                    newBullet -> dy = -10;
                    newBullet -> isMain = true;
                    newBullet -> isActive = true;

                    newBullet -> next = bulletHead;

                    bulletHead = newBullet;

                    player.inInterval = true;
                    player.bulletsNum--;
                    
                }
            }

            if(e.type == ClientMessage){
                if ((Atom)e.xclient.data.l[0] == wmDeleteMessage){
                    goto END_GAME;
                }
            }
        }

        CreateEnemy(difficulty, &enemyHead);

        CreateEBullet(&enemyHead, &bulletHead);
        
        UpdateEnemyRotation(&enemyHead, player);

        //描画処理
        XClearWindow(d, w);
        XSetForeground(d, DefaultGC(d, s), WhitePixel(d, s));
        DrawPlayer(d, w, s, player.x, player.y);
        DrawEnemy(d, w, s, enemyHead);
        DrawBullet(d, w, s, bulletHead);
        DrawSupply(d, w, s, supplyHead);
        DrawUI(d, w, s, player, score, fontBig, fontSmall);
        
        
        XFlush(d);

        ResolveCollisions(&player, enemyHead, bulletHead, supplyHead, &score);

        FreeDisActiveBullets(&bulletHead);
        FreeDisActiveEnemys(&enemyHead, &supplyHead);
        FreeDisActiveSupplies(&supplyHead);

        //弾丸のクールタイム処理
        if(player.inInterval){
            player.interval++;

            if(player.interval ==10){
                player.inInterval = false;
                player.interval = 0;
            }
        }

        

        //ゲームオーバー処理
        if(player.heart <= 0){
            player.heart = 0;
            XClearWindow(d, w);
            XSetForeground(d, DefaultGC(d, s), WhitePixel(d, s));
            DrawPlayer(d, w, s, player.x, player.y);
            DrawEnemy(d, w, s, enemyHead);
            DrawBullet(d, w, s, bulletHead);
            DrawSupply(d, w, s, supplyHead);
            DrawUI(d, w, s, player, score, fontBig, fontSmall);

            DrawGameOverAnimation(d, w, s, player);
            FreeAllBullets(bulletHead);
            FreeAllEnemies(enemyHead);
            FreeAllSupplies(supplyHead);

            bulletHead = NULL;
            enemyHead = NULL;
            supplyHead = NULL;

            if(score >= highScore){
                fp = fopen(data_file, "w");
                fprintf(fp, "%d", score);
                fclose(fp);
            }

            goto TITLE;
        }

        elapsedTime++;
        if(elapsedTime == 600){
            elapsedTime = 0;
            difficulty++;
        }


        usleep(30000);
    }

    END_GAME:

    FreeAllBullets(bulletHead);
    FreeAllEnemies(enemyHead);
    FreeAllSupplies(supplyHead);
    
    printf("正常終了します。\n");

    XCloseDisplay(d);
    return 0;
}
