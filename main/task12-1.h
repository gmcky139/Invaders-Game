#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdbool.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define PI 3.14159265

struct Player{
    int x, y;
    int size;
    int speed;
	int bulletsNum;
	int heart;
    int interval;
    bool inInterval;
};

typedef struct Enemy{
    double originalX, originalY;
    double x[6];
    double y[6];
    double dx, dy;
	int bulletNum;
	int heart;
	bool doSpecialAttack;
    int interval;
    int time;
    bool isActive;

    struct Enemy* next;
} Enemy;

typedef struct Bullet{
    int x, y;
    int dy;
    bool isMain;
    bool isActive;

    struct Bullet* next;
} Bullet;

typedef struct Supply{
    double x, y, dy;
    int bullets;
    int heart;
    bool isActive;

    struct Supply* next;
} Supply;


void InitializePlayer(struct Player* player);
void DrawPlayer(Display *d, Window w, int s, int x, int y);
void DrawEnemy(Display *d, Window w, int s, Enemy *head);
void DrawBullet(Display *d, Window w, int s, Bullet *head);
void DrawSupply(Display *d, Window w, int s, Supply *head);
void DrawUI(Display *d, Window w, int s, struct Player player, int score, XFontStruct *fBig, XFontStruct *fSmal);
void DrawGameOverAnimation(Display *d, Window w, int s, struct Player player);
void CreateEnemy(int difficulty, Enemy **headRef);
void CreateEBullet(Enemy **eHeadRef, Bullet **bHeadRef);
void ResolveCollisions(struct Player *player, Enemy *enemyHead, Bullet *bulletHead, Supply *supplyHead, int *score);
int CheckRectCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
int CheckCircleCollision(double x1, double y1, double r1, double x2, double y2, double r2);
int CheckRectCircleCollision(double rx, double ry, double w, double h, double cx, double cy, double r);
void UpdateEnemyRotation(Enemy **enemyHeadRef, struct Player player);
void FreeDisActiveEnemys(Enemy **eHeadRef, Supply **sHeadRef);
void FreeDisActiveBullets(Bullet **headRef);
void FreeDisActiveSupplies(Supply **hedRef);
void FreeAllBullets(Bullet *head);
void FreeAllEnemies(Enemy *head);
void FreeAllSupplies(Supply *head);