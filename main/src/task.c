#include "task.h"

void InitializePlayer(struct Player *player){
    player -> bulletsNum = 30;
    player -> heart = 100;
    player -> x = 320;
    player -> y = 710;
    player -> speed = 10;
    player -> size = 40;
    player -> interval = 0;
    player -> inInterval = false;
}

void DrawPlayer(Display *d, Window w, int s, int x, int y){
    XPoint points[3];

    points[0].x = x - 35;
    points[0].y = y + 8;
    points[1].x = x - 10;
    points[1].y = y - 5;
    points[2].x = x - 10;
    points[2].y = y + 8;
    XFillPolygon(d, w, DefaultGC(d, s), points, 3, Convex, CoordModeOrigin);

    points[0].x = x + 35;
    points[0].y = y + 8;
    points[1].x = x + 10;
    points[1].y = y - 5;
    points[2].x = x + 10;
    points[2].y = y + 8;
    XFillPolygon(d, w, DefaultGC(d, s), points, 3, Convex, CoordModeOrigin);

    points[0].x = x - 15;
    points[0].y = y + 8;
    points[1].x = x - 9;
    points[1].y = y - 10;
    points[2].x = x - 9;
    points[2].y = y + 8;
    XFillPolygon(d, w, DefaultGC(d, s), points, 3, Convex, CoordModeOrigin);

    points[0].x = x + 15;
    points[0].y = y + 8;
    points[1].x = x + 9;
    points[1].y = y - 10;
    points[2].x = x + 9;
    points[2].y = y + 8;
    XFillPolygon(d, w, DefaultGC(d, s), points, 3, Convex, CoordModeOrigin);

    XFillRectangle(d, w, DefaultGC(d, s), x - 35, y + 8, 71, 5);
    XFillRectangle(d, w, DefaultGC(d, s), x -9, y - 10, 19, 30);
    XFillRectangle(d, w, DefaultGC(d, s), x - 27, y - 3, 5, 14);
    XFillRectangle(d, w, DefaultGC(d, s), x + 23, y - 3, 5, 14);

    XFillArc(d, w, DefaultGC(d, s), x - 9, y - 20, 18, 20, 0, 360 * 64);
}

void DrawEnemy(Display *d, Window w, int s, Enemy *head){
    for(Enemy *current = head; current != NULL; current = current -> next){
        current -> originalX += current -> dx;
        current -> originalY += current -> dy;
        double oriX = current -> originalX;
        double oriY = current -> originalY;
        double* X = current -> x;
        double* Y = current -> y;

        for(int i = 0; i < 6; i++){
            XDrawLine(d, w, DefaultGC(d, s), (int)(oriX + X[i]) , (int)(oriY + Y[i]), (int)(oriX + X[(i + 1)%6]), (int)(oriY + Y[(i + 1)%6]));
        }
    }
}

void DrawBullet(Display *d, Window w, int s, Bullet *head){
    for(Bullet *current = head; current != NULL; current = current -> next){
        current -> y += current -> dy;

        if(current -> isMain){
            XFillRectangle(d, w, DefaultGC(d, s), (int)current->x, (int)current->y, 9, 30);
        }
        else{
            XDrawRectangle(d, w, DefaultGC(d, s), (int)current->x, (int)current->y, 9, 30);
        }
    }
}

void DrawSupply(Display *d, Window w, int s, Supply *head){
    for(Supply *current = head; current != NULL; current = current -> next){
        current -> y += current -> dy;

        XFillRectangle(d, w, DefaultGC(d, s), (int)current->x - 12, (int)current->y - 12, 25, 25);
    }
}

void DrawUI(Display *d, Window w, int s, struct Player player, int score, XFontStruct *fBig, XFontStruct *fSmall){
    XSetForeground(d, DefaultGC(d, s), BlackPixel(d, s));
    XFillRectangle(d, w, DefaultGC(d, s), 0, 748, 639, 71);
    XSetForeground(d, DefaultGC(d, s), WhitePixel(d, s));
    XDrawRectangle(d, w, DefaultGC(d, s), 0, 750, 639, 69);
    XDrawLine(d, w, DefaultGC(d, s), 540, 750, 540, 819);

    //体力バー
    XFillRectangle(d, w, DefaultGC(d, s), 50, 770, (int)(4.5 * player.heart), 20);

    XSetFont(d, DefaultGC(d, s), fSmall -> fid);

    char str1[] = "GUN";
    XDrawString(d, w, DefaultGC(d, s), 550, 775, str1, strlen(str1));

    char hp[15];
    sprintf(hp, "%3d / 100", player.heart);
    XDrawString(d, w, DefaultGC(d, s), 50, 810, hp, strlen(hp));

    char scr[30];
    sprintf(scr, "Score: %010d", score);
    XDrawString(d, w, DefaultGC(d, s), 20, 20, scr, strlen(scr));

    XSetFont(d, DefaultGC(d, s), fBig -> fid);

    char str2[] = "HP";
    XDrawString(d, w, DefaultGC(d, s), 10, 790, str2, strlen(str2));

    char mag[10];
    sprintf(mag, "x%d", player.bulletsNum);
    XDrawString(d, w, DefaultGC(d, s), 550, 810, mag, strlen(mag));

    
}

void DrawGameOverAnimation(Display *d, Window w, int s, struct Player player){
    for(int i = 0; i < 4; i++){
        int r = 20 * i;
        int diameter = r * 2;

        XSetForeground(d, DefaultGC(d, s), WhitePixel(d, s));
        XFillArc(d, w, DefaultGC(d, s), player.x - r, player.y - r,  diameter, diameter, 0, 360 * 64);

        r = 10 * i;      
        diameter = r * 2;

        XSetForeground(d, DefaultGC(d, s), BlackPixel(d, s));
        XFillArc(d, w, DefaultGC(d, s), player.x - r, player.y - r,  diameter, diameter, 0, 360 * 64);

        XFlush(d);
        usleep(500000);
    }
    
    usleep(300000);
}

void CreateEnemy(int difficulty, Enemy **headRef){
    if(rand() % (int)(30 + 40 * pow(0.9, (double)difficulty)) == 1){
        Enemy* newEnemy = (Enemy*)malloc(sizeof(Enemy));

        if(newEnemy != NULL){
            newEnemy -> originalX = rand() % 560 + 40;
            newEnemy -> originalY = -20;
            newEnemy -> dx = 0;
            newEnemy -> dy = (rand() % 5 + 5) / 10.0;
            newEnemy -> bulletNum = rand() % 7 + 5;
            newEnemy -> heart = 3;
            newEnemy -> doSpecialAttack = rand() % 2;
            newEnemy -> interval = 60;
            newEnemy -> time = 0;
            newEnemy -> next = *headRef;
            newEnemy -> isActive = true;

            double tempX[6] = {0, -35, -35, 0, 35, 35};
            memcpy(newEnemy -> x, tempX, sizeof(tempX));
            double tempY[6] = {15, -5, -15, -20, -15, -5};
            memcpy(newEnemy -> y, tempY, sizeof(tempY));

            *headRef = newEnemy;
        }
    }
}

void CreateEBullet(Enemy **eHeadRef, Bullet **bHeadRef){
    for(Enemy* current = *eHeadRef; current != NULL; current = current -> next){
            current -> time++;

            if(current -> time % current -> interval == 0 &&current -> bulletNum > 0){
                current -> time = 0;

                Bullet* newBullet = (Bullet*)malloc(sizeof(Bullet));

                if(newBullet != NULL){
                    newBullet -> x = current -> originalX + current -> x[0] - 4;
                    newBullet -> y = current -> originalY + current -> y[0] + 5;
                    newBullet -> dy = 10;
                    newBullet -> isMain = false;
                    newBullet -> isActive = true;

                    newBullet -> next = *bHeadRef;

                    *bHeadRef = newBullet;

                    current -> bulletNum--;
                }
            }
        }
}

void ResolveCollisions(struct Player *player, Enemy *enemyHead, Bullet *bulletHead, Supply *supplyHead, int *score){
    struct Player p = *player;
    
    //弾丸の接触処理
    for(Bullet *currentB = bulletHead; currentB != NULL; currentB = currentB -> next){
        //弾丸と敵の接触処理
        for(Enemy *currentE = enemyHead; currentE != NULL; currentE = currentE -> next){
            if(!currentB -> isActive){continue;}
            if(!currentE -> isActive){continue;}
            
            if(currentB -> isMain && (CheckRectCircleCollision(currentB -> x, currentB -> y, 9, 30, currentE -> originalX, currentE -> originalY, 15))){
                currentB -> isActive = false;
                currentE -> heart--;

                if(currentE -> heart <= 0){
                    if(currentE -> bulletNum == 0){
                        *score += 500;
                    }
                    else{
                        *score += 1000;
                    }
                }
            }
        }

        if(!currentB -> isMain && (CheckRectCircleCollision(currentB -> x, currentB -> y, 9, 30, (double)(p.x), (double)(p.y) - 10, 10) || CheckRectCollision(currentB -> x, currentB -> y, 9, 30, p.x -9, p.y - 10, 19, 30))){
            player -> heart -= 30;
            currentB -> isActive = false;
        }
        else if(!currentB -> isMain && CheckRectCollision(currentB -> x, currentB -> y, 9, 30, p.x - 27, p.y - 3, 55, 14)){
            player -> heart -= 10;
            currentB -> isActive = false;
        }
    }

    //敵とプレイヤーの接触処理
    for(Enemy *current = enemyHead; current != NULL; current = current -> next){
        if(CheckCircleCollision(current -> originalX, current -> originalY, 15, (double)(p.x), (double)(p.y) - 10, 10) || CheckRectCircleCollision(p.x - 27, p.y - 3, 55, 14, current -> originalX, current -> originalY, 15)){
            player -> heart -= 90;
            current -> isActive = false;
        }
        else if(CheckRectCircleCollision(p.x - 35, p.y - 3, 71, 14, current -> originalX, current -> originalY, 15)){
            player -> heart -= 40;
            current -> isActive = false;
        }
    }

    //補給物資とプレイヤーの接触処理
    for(Supply *current = supplyHead; current != NULL; current = current -> next){
        if(CheckRectCircleCollision(current -> x - 12, current -> y - 12, 25, 25, (double)(p.x), (double)(p.y) - 10, 10) || CheckRectCollision(current -> x - 12, current -> y - 12, 25, 25, p.x -9, p.y - 10, 19, 30) || CheckRectCollision(current -> x - 12, current -> y - 12, 25, 25, p.x - 27, p.y - 3, 55, 14)){
            player -> bulletsNum += current -> bullets;
            player -> heart += current -> heart;
            current -> isActive = false;

            if(player -> heart > 100){
                player -> heart = 100;
            }

            *score +=150;
        }
    }
    
}

int CheckRectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
    if (x1 < x2 + w2 && x2 < x1 + w1) {
        if (y1 < y2 + h2 && y2 < y1 + h1) {
            return 1; 
        }
    }

    return 0;
}

int CheckCircleCollision(double x1, double y1, double r1, double x2, double y2, double r2){
    if((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) < (r1 + r2)*(r1 + r2)){
        return 1;
    }
    else{
        return 0;
    }
}

int CheckRectCircleCollision(double rx, double ry, double w, double h, double cx, double cy, double r){
    double closestX = cx;
    double closestY = cy;

    if (closestX < rx) {
        closestX = rx;
    } else if (closestX > rx + w) {
        closestX = rx + w;
    }

    if (closestY < ry) {
        closestY = ry;
    } else if (closestY > ry + h) {
        closestY = ry + h;
    }

    if ((cx - closestX) * (cx - closestX) + (cy - closestY) * (cy - closestY) < r * r){
        return 1; 
    }
    else{
        return 0;
    }
}

void UpdateEnemyRotation(Enemy **enemyHeadRef, struct Player player){
    Enemy *current = *enemyHeadRef;

    while (current != NULL){
        double rad = 0;
        if((double)(player.y) > current -> originalY){
            rad = (atan2((double)(player.y) - current -> originalY, (double)(player.x) - current -> originalX) - atan2(current -> y[0], current -> x[0])) / 100.0;
        }
        if(current -> bulletNum == 0 && current -> doSpecialAttack){

            for(int i = 0; i < 6; i++){
                double x = current -> x[i];
                double y = current -> y[i];
                current -> x[i] = x * cos(rad) - y * sin(rad);
                current -> y[i] = x * sin(rad) + y * cos(rad);
            }

            current -> dy += 0.01;
            current -> dx -= 2 * tan(rad) / current -> dy;
        }

        current = current -> next;
    }
    
}

void FreeDisActiveBullets(Bullet **headRef){
    Bullet *current = *headRef;
    Bullet *prev = NULL;

    while (current != NULL){
        if(current -> y < -40 || current -> y > 860 || !current -> isActive){
            Bullet* temp = current;

            if(prev == NULL){
                *headRef = current -> next;
                current = *headRef;
            }
            else{
                prev -> next = current -> next;
                current = current -> next;
            }
            free(temp);
        }
        else{
            prev = current;
            current = current -> next;
        }
    }
}

void FreeDisActiveEnemys(Enemy **eHeadRef, Supply **sHeadRef){
    Enemy *current = *eHeadRef;
    Enemy *prev = NULL;

    while (current != NULL){
        if(current -> originalY > 860 || current -> heart <= 0 || !current -> isActive){

            if(current -> heart <= 0){
                Supply *newSupply = (Supply*)malloc(sizeof(Supply));

                if(newSupply != NULL){
                    newSupply -> x = current -> originalX;
                    newSupply -> y = current -> originalY;
                    newSupply -> dy = 0.5;
                    newSupply -> bullets = rand() % 10 + 15;
                    newSupply -> heart = rand() % 45 + 5;
                    newSupply -> isActive = true;

                    newSupply -> next = *sHeadRef;

                    *sHeadRef = newSupply;
                }
            }
            Enemy *temp = current;

            if(prev == NULL){
                *eHeadRef = current -> next;
                current = *eHeadRef;
            }
            else{
                prev -> next = current -> next;
                current = current -> next;
            }
            free(temp);
        }
        else{
            prev = current;
            current = current -> next;
        }
    }
}

void FreeDisActiveSupplies(Supply **headRef){
    Supply *current = *headRef;
    Supply *prev = NULL;

    while (current != NULL){
        if(current -> y > 860 || !current -> isActive){
            Supply *temp = current;

            if(prev == NULL){
                *headRef = current -> next;
                current = *headRef;
            }
            else{
                prev -> next = current -> next;
                current = current -> next;
            }
            free(temp);
        }
        else{
            prev = current;
            current = current -> next;
        }
    }
}

void FreeAllBullets(Bullet *head) {
    Bullet *current = head;
    while (current != NULL){
        Bullet *nextInfo = current -> next;
        free(current);
        current = nextInfo;
    }
}

void FreeAllEnemies(Enemy *head) {
    Enemy *current = head;
    while (current != NULL){
        Enemy *nextInfo = current -> next;
        free(current);
        current = nextInfo;
    }
}

void FreeAllSupplies(Supply *head) {
    Supply *current = head;
    while (current != NULL){
        Supply *nextInfo = current -> next;
        free(current);
        current = nextInfo;
    }
}