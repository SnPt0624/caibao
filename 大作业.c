#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>  

#define ZUIDAJILU 1000
#define ZUIDAYONGHU 100
#define ZUIDAMINGCHANG 20
#define ZUIDAMIMA 20
#define ZUIDALEIBIE 20
#define ZUIDABEIZHU 50

typedef struct {
    char yonghuming[ZUIDAMINGCHANG];
    char mima[ZUIDAMIMA];
} YongHu;

typedef struct {
    int leixing;
    float jine;
    char leibie[ZUIDALEIBIE];
    char shijian[20];
    char beizhu[ZUIDABEIZHU];
} XiaoFeiJiLu;

typedef struct {
    YongHu yonghushuzu[ZUIDAYONGHU];
    XiaoFeiJiLu xiaofeijilu[ZUIDAJILU];
    int yonghushu;
    int jilushu;
    int dangqianyonghu;
} XiTongShuJu;


void shurumima(char *mima, int maxlen) {
    int i = 0;
    char ch;
    while (1) {
        ch = _getch();
        if (ch == '\r') {  
            mima[i] = '\0';
            break;
        } else if (ch == '\b' && i > 0) {  
            i--;
            printf("\b \b"); 
        } else if (ch >= 32 && ch <= 126 && i < maxlen - 1) { 
            mima[i++] = ch;
            printf("*"); 
        }
    }
    printf("\n");
}


void zhuceyonghu(XiTongShuJu *xtsj) {
    if (xtsj->yonghushu >= ZUIDAYONGHU) {
        printf("用户数量已达上限，无法注册！\n");
        return;
    }
    YongHu xinyonghu;
    printf("===== 用户注册 =====\n");
    printf("请输入用户名：");
    scanf("%s", xinyonghu.yonghuming);

    for (int i = 0; i < xtsj->yonghushu; i++) {
        if (strcmp(xtsj->yonghushuzu[i].yonghuming, xinyonghu.yonghuming) == 0) {
            printf("用户名已存在！\n");
            return;
        }
    }
    printf("请输入密码：");
    shurumima(xinyonghu.mima, ZUIDAMIMA); 

    xtsj->yonghushuzu[xtsj->yonghushu++] = xinyonghu;
    baocunyonghuerjinzhi(xtsj);
    printf("注册成功！");
}

void baocunyonghuerjinzhi(XiTongShuJu *xtsj) {
    FILE *fp = fopen("yonghu.bin", "wb");
    if (!fp) {
        printf("保存用户失败！\n");
        return;
    }
    fwrite(&xtsj->yonghushu, sizeof(int), 1, fp);
    fwrite(xtsj->yonghushuzu, sizeof(YongHu), xtsj->yonghushu, fp);
    fclose(fp);
}

int jiazaiyonghuerjinzhi(XiTongShuJu *xtsj) {
    FILE *fp = fopen("yonghu.bin", "rb");
    if (!fp) return 0;
    fread(&xtsj->yonghushu, sizeof(int), 1, fp);
    fread(xtsj->yonghushuzu, sizeof(YongHu), xtsj->yonghushu, fp);
    fclose(fp);
    return xtsj->yonghushu;
}

void tianjiajilu(XiTongShuJu *xtsj) {
    if (xtsj->jilushu >= ZUIDAJILU) {
        printf("记录数量已达上限！\n");
        return;
    }
    XiaoFeiJiLu xinjilu;
    printf("===== 添加消费记录 =====\n");
    printf("请选择类型（0-支出，1-收入）：");
    scanf("%d", &xinjilu.leixing);
    if (xinjilu.leixing != 0 && xinjilu.leixing != 1) {
        printf("类型错误！\n");
        return;
    }
    printf("请输入金额：");
    scanf("%f", &xinjilu.jine);
    printf("请输入类别：");
    scanf("%s", xinjilu.leibie);
    huoqudangqianshijian(xinjilu.shijian);
    printf("请输入备注：");
    scanf("%s", xinjilu.beizhu);

    xtsj->xiaofeijilu[xtsj->jilushu++] = xinjilu;
    baocunjiluerjinzhi(xtsj);
    printf("记录添加成功！\n");
}

void huoqudangqianshijian(char *sjstr) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(sjstr, "%04d-%02d-%02d %02d:%02d",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min);
}

void baocunjiluerjinzhi(XiTongShuJu *xtsj) {
    char wenjianming[50];
    sprintf(wenjianming, "jilu%s.bin", xtsj->yonghushuzu[xtsj->dangqianyonghu].yonghuming);
    FILE *fp = fopen(wenjianming, "wb");
    if (!fp) {
        printf("保存记录失败！\n");
        return;
    }
    fwrite(&xtsj->jilushu, sizeof(int), 1, fp);
    fwrite(xtsj->xiaofeijilu, sizeof(XiaoFeiJiLu), xtsj->jilushu, fp);
    fclose(fp);
}

int jiazaijiluerjinzhi(XiTongShuJu *xtsj) {
    char wenjianming[50];
    sprintf(wenjianming, "jilu%s.bin", xtsj->yonghushuzu[xtsj->dangqianyonghu].yonghuming);
    FILE *fp = fopen(wenjianming, "rb");
    if (!fp) {
        xtsj->jilushu = 0;
        return 0;
    }
    fread(&xtsj->jilushu, sizeof(int), 1, fp);
    fread(xtsj->xiaofeijilu, sizeof(XiaoFeiJiLu), xtsj->jilushu, fp);
    fclose(fp);
    return xtsj->jilushu;
}

void chaxunjilu(XiTongShuJu *xtsj) {
    int xuanze;
    printf("===== 消费记录查询 =====\n");
    printf("1. 查询所有记录\n");
    printf("2. 按类别查询\n");
    printf("3. 按金额范围查询\n");
    printf("请选择：");
    scanf("%d", &xuanze);
    if (xtsj->jilushu == 0) {
        printf("暂无记录！\n");
        return;
    }
    switch (xuanze) {
        case 1:
            printf("\n类型\t金额\t类别\t时间\t\t备注\n");
            for (int i = 0; i < xtsj->jilushu; i++) {
                printf("%s\t%.2f\t%s\t%s\t%s\n",
                       xtsj->xiaofeijilu[i].leixing ? "收入" : "支出",
                       xtsj->xiaofeijilu[i].jine,
                       xtsj->xiaofeijilu[i].leibie,
                       xtsj->xiaofeijilu[i].shijian,
                       xtsj->xiaofeijilu[i].beizhu);
            }
            break;
        case 2: {
            char leibie[ZUIDALEIBIE];
            printf("类别：");
            scanf("%s", leibie);
            printf("\n类型\t金额\t时间\t\t备注\n");
            int zhaodao = 0;
            for (int i = 0; i < xtsj->jilushu; i++) {
                if (strcmp(xtsj->xiaofeijilu[i].leibie, leibie) == 0) {
                    printf("%s\t%.2f\t%s\t%s\n",
                           xtsj->xiaofeijilu[i].leixing ? "收入" : "支出",
                           xtsj->xiaofeijilu[i].jine,
                           xtsj->xiaofeijilu[i].shijian,
                           xtsj->xiaofeijilu[i].beizhu);
                    zhaodao = 1;
                }
            }
            if (!zhaodao) printf("无结果\nn");
            break;
        }
        case 3: {
            float zuixiao, zuida;
            printf("最小金额："); scanf("%f", &zuixiao);
            printf("最大金额："); scanf("%f", &zuida);
            printf("\n类型\t金额\t类别\t时间\t\t备注\n");
            int zhaodao = 0;
            for (int i = 0; i < xtsj->jilushu; i++) {
                if (xtsj->xiaofeijilu[i].jine >= zuixiao && xtsj->xiaofeijilu[i].jine <= zuida) {
                    printf("%s\t%.2f\t%s\t%s\t%s\n",
                           xtsj->xiaofeijilu[i].leixing ? "收入" : "支出",
                           xtsj->xiaofeijilu[i].jine,
                           xtsj->xiaofeijilu[i].leibie,
                           xtsj->xiaofeijilu[i].shijian,
                           xtsj->xiaofeijilu[i].beizhu);
                    zhaodao = 1;
                }
            }
            if (!zhaodao) printf("无结果\n");
            break;
        }
        default:
            printf("输入错误\n");
    }
}

void xianshicaidan() {
    printf("\n===== 学生个人消费管理系统 =====\n");
    printf("1. 注册\n");
    printf("2. 登录\n");
    printf("3. 退出\n");
    printf("请选择：");
}

int dengluyonghu(XiTongShuJu *xtsj) {
    char yhm[ZUIDAMINGCHANG], mm[ZUIDAMIMA];
    printf("===== 登录 =====\n");
    printf("用户名"); scanf("%s", yhm);
    printf("密码：");
    shurumima(mm, ZUIDAMIMA); 

    for (int i = 0; i < xtsj->yonghushu; i++) {
        if (strcmp(xtsj->yonghushuzu[i].yonghuming, yhm) == 0) {
            if (strcmp(xtsj->yonghushuzu[i].mima, mm) == 0) {
                xtsj->dangqianyonghu = i;
                jiazaijiluerjinzhi(xtsj);
                printf("登录成功\n");
                return 1;
            } else {
                printf("密码错误\n");
                return 0;
            }
        }
    }
    printf("用户不存在\n");
    return 0;
}

void yonghucaidan(XiTongShuJu *xtsj) {
    int xuanze;
    while (1) {
        printf("\n1. 添加消费记录\n2. 查询记录\n3. 返回主菜单\n选择：");
        scanf("%d", &xuanze);
        switch (xuanze) {
            case 1: tianjiajilu(xtsj); break;
            case 2: chaxunjilu(xtsj); break;
            case 3:
                xtsj->dangqianyonghu = -1;
                xtsj->jilushu = 0;
                return;
            default:
                printf("输入错误\n");
        }
    }
}

int main() {
    XiTongShuJu xtsj = {0};
    jiazaiyonghuerjinzhi(&xtsj);
    int xuanze;
    while (1) {
        xianshicaidan();
        scanf("%d", &xuanze);
        switch (xuanze) {
            case 1: zhuceyonghu(&xtsj); break;
            case 2:
                if (dengluyonghu(&xtsj)) yonghucaidan(&xtsj);
                break;
            case 3:
                printf("退出系统\n");
                return 0;
            default:
                printf("输入错误\n");
        }
    }
    return 0;
}
