#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>  

#define ZUI_DA_JI_LU 1000
#define ZUI_DA_YONG_HU 100
#define ZUI_DA_MING_CHANG 20
#define ZUI_DA_MI_MA 20
#define ZUI_DA_LEI_BIE 20
#define ZUI_DA_BEI_ZHU 50

typedef struct {
    char yong_hu_ming[ZUI_DA_MING_CHANG];
    char mi_ma[ZUI_DA_MI_MA];
} YongHu;

typedef struct {
    int lei_xing;
    float jin_e;
    char lei_bie[ZUI_DA_LEI_BIE];
    char shi_jian[20];
    char bei_zhu[ZUI_DA_BEI_ZHU];
} XiaoFeiJiLu;

typedef struct {
    YongHu yong_hu_shu_zu[ZUI_DA_YONG_HU];
    XiaoFeiJiLu xiao_fei_ji_lu[ZUI_DA_JI_LU];
    int yong_hu_shu;
    int ji_lu_shu;
    int dang_qian_yong_hu;
} XiTongShuJu;


void shu_ru_mi_ma(char *mi_ma, int max_len) {
    int i = 0;
    char ch;
    while (1) {
        ch = _getch(); 
        if (ch == '\r') {  
            mi_ma[i] = '\0';
            break;
        } else if (ch == '\b' && i > 0) {  
            i--;
            printf("\b \b"); 
        } else if (ch >= 32 && ch <= 126 && i < max_len - 1) { 
            mi_ma[i++] = ch;
            printf("*"); 
        }
    }
    printf("\n");
}


void zhu_ce_yong_hu(XiTongShuJu *xtsj) {
    if (xtsj->yong_hu_shu >= ZUI_DA_YONG_HU) {
        printf("用户数量已达上限，无法注册！\n");
        return;
    }
    YongHu xin_yong_hu;
    printf("===== 用户注册 =====\n");
    printf("请输入用户名：");
    scanf("%s", xin_yong_hu.yong_hu_ming);

    for (int i = 0; i < xtsj->yong_hu_shu; i++) {
        if (strcmp(xtsj->yong_hu_shu_zu[i].yong_hu_ming, xin_yong_hu.yong_hu_ming) == 0) {
            printf("用户名已存在！\n");
            return;
        }
    }
    printf("请输入密码：");
    shu_ru_mi_ma(xin_yong_hu.mi_ma, ZUI_DA_MI_MA); 

    xtsj->yong_hu_shu_zu[xtsj->yong_hu_shu++] = xin_yong_hu;
    bao_cun_yong_hu_er_jin_zhi(xtsj);
    printf("注册成功！\n");
}

void bao_cun_yong_hu_er_jin_zhi(XiTongShuJu *xtsj) {
    FILE *fp = fopen("yonghu.bin", "wb");
    if (!fp) {
        printf("保存用户失败！\n");
        return;
    }
    fwrite(&xtsj->yong_hu_shu, sizeof(int), 1, fp);
    fwrite(xtsj->yong_hu_shu_zu, sizeof(YongHu), xtsj->yong_hu_shu, fp);
    fclose(fp);
}

int jia_zai_yong_hu_er_jin_zhi(XiTongShuJu *xtsj) {
    FILE *fp = fopen("yonghu.bin", "rb");
    if (!fp) return 0;
    fread(&xtsj->yong_hu_shu, sizeof(int), 1, fp);
    fread(xtsj->yong_hu_shu_zu, sizeof(YongHu), xtsj->yong_hu_shu, fp);
    fclose(fp);
    return xtsj->yong_hu_shu;
}

void tian_jia_ji_lu(XiTongShuJu *xtsj) {
    if (xtsj->ji_lu_shu >= ZUI_DA_JI_LU) {
        printf("记录数量已达上限！\n");
        return;
    }
    XiaoFeiJiLu xin_ji_lu;
    printf("===== 添加消费记录 =====\n");
    printf("请选择类型（0-支出，1-收入）：");
    scanf("%d", &xin_ji_lu.lei_xing);
    if (xin_ji_lu.lei_xing != 0 && xin_ji_lu.lei_xing != 1) {
        printf("类型错误！\n");
        return;
    }
    printf("请输入金额：");
    scanf("%f", &xin_ji_lu.jin_e);
    printf("请输入类别：");
    scanf("%s", xin_ji_lu.lei_bie);
    huo_qu_dang_qian_shi_jian(xin_ji_lu.shi_jian);
    printf("请输入备注：");
    scanf("%s", xin_ji_lu.bei_zhu);

    xtsj->xiao_fei_ji_lu[xtsj->ji_lu_shu++] = xin_ji_lu;
    bao_cun_ji_lu_er_jin_zhi(xtsj);
    printf("记录添加成功！\n");
}

void huo_qu_dang_qian_shi_jian(char *sj_str) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(sj_str, "%04d-%02d-%02d %02d:%02d",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min);
}

void bao_cun_ji_lu_er_jin_zhi(XiTongShuJu *xtsj) {
    char wen_jian_ming[50];
    sprintf(wen_jian_ming, "jilu_%s.bin", xtsj->yong_hu_shu_zu[xtsj->dang_qian_yong_hu].yong_hu_ming);
    FILE *fp = fopen(wen_jian_ming, "wb");
    if (!fp) {
        printf("保存记录失败！\n");
        return;
    }
    fwrite(&xtsj->ji_lu_shu, sizeof(int), 1, fp);
    fwrite(xtsj->xiao_fei_ji_lu, sizeof(XiaoFeiJiLu), xtsj->ji_lu_shu, fp);
    fclose(fp);
}

int jia_zai_ji_lu_er_jin_zhi(XiTongShuJu *xtsj) {
    char wen_jian_ming[50];
    sprintf(wen_jian_ming, "jilu_%s.bin", xtsj->yong_hu_shu_zu[xtsj->dang_qian_yong_hu].yong_hu_ming);
    FILE *fp = fopen(wen_jian_ming, "rb");
    if (!fp) {
        xtsj->ji_lu_shu = 0;
        return 0;
    }
    fread(&xtsj->ji_lu_shu, sizeof(int), 1, fp);
    fread(xtsj->xiao_fei_ji_lu, sizeof(XiaoFeiJiLu), xtsj->ji_lu_shu, fp);
    fclose(fp);
    return xtsj->ji_lu_shu;
}

void cha_xun_ji_lu(XiTongShuJu *xtsj) {
    int xuan_ze;
    printf("===== 消费记录查询 =====\n");
    printf("1. 查询所有记录\n");
    printf("2. 按类别查询\n");
    printf("3. 按金额范围查询\n");
    printf("请选择：");
    scanf("%d", &xuan_ze);
    if (xtsj->ji_lu_shu == 0) {
        printf("暂无记录！\n");
        return;
    }
    switch (xuan_ze) {
        case 1:
            printf("\n类型\t金额\t类别\t时间\t\t备注\n");
            for (int i = 0; i < xtsj->ji_lu_shu; i++) {
                printf("%s\t%.2f\t%s\t%s\t%s\n",
                       xtsj->xiao_fei_ji_lu[i].lei_xing ? "收入" : "支出",
                       xtsj->xiao_fei_ji_lu[i].jin_e,
                       xtsj->xiao_fei_ji_lu[i].lei_bie,
                       xtsj->xiao_fei_ji_lu[i].shi_jian,
                       xtsj->xiao_fei_ji_lu[i].bei_zhu);
            }
            break;
        case 2: {
            char lei_bie[ZUI_DA_LEI_BIE];
            printf("类别：");
            scanf("%s", lei_bie);
            printf("\n类型\t金额\t时间\t\t备注\n");
            int zhao_dao = 0;
            for (int i = 0; i < xtsj->ji_lu_shu; i++) {
                if (strcmp(xtsj->xiao_fei_ji_lu[i].lei_bie, lei_bie) == 0) {
                    printf("%s\t%.2f\t%s\t%s\n",
                           xtsj->xiao_fei_ji_lu[i].lei_xing ? "收入" : "支出",
                           xtsj->xiao_fei_ji_lu[i].jin_e,
                           xtsj->xiao_fei_ji_lu[i].shi_jian,
                           xtsj->xiao_fei_ji_lu[i].bei_zhu);
                    zhao_dao = 1;
                }
            }
            if (!zhao_dao) printf("无结果\n");
            break;
        }
        case 3: {
            float zui_xiao, zui_da;
            printf("最小金额："); scanf("%f", &zui_xiao);
            printf("最大金额："); scanf("%f", &zui_da);
            printf("\n类型\t金额\t类别\t时间\t\t备注\n");
            int zhao_dao = 0;
            for (int i = 0; i < xtsj->ji_lu_shu; i++) {
                if (xtsj->xiao_fei_ji_lu[i].jin_e >= zui_xiao && xtsj->xiao_fei_ji_lu[i].jin_e <= zui_da) {
                    printf("%s\t%.2f\t%s\t%s\t%s\n",
                           xtsj->xiao_fei_ji_lu[i].lei_xing ? "收入" : "支出",
                           xtsj->xiao_fei_ji_lu[i].jin_e,
                           xtsj->xiao_fei_ji_lu[i].lei_bie,
                           xtsj->xiao_fei_ji_lu[i].shi_jian,
                           xtsj->xiao_fei_ji_lu[i].bei_zhu);
                    zhao_dao = 1;
                }
            }
            if (!zhao_dao) printf("无结果\n");
            break;
        }
        default:
            printf("输入错误\n");
    }
}

void xian_shi_cai_dan() {
    printf("\n===== 学生个人消费管理系统 =====\n");
    printf("1. 注册\n");
    printf("2. 登录\n");
    printf("3. 退出\n");
    printf("请选择：");
}

int deng_lu_yong_hu(XiTongShuJu *xtsj) {
    char yhm[ZUI_DA_MING_CHANG], mm[ZUI_DA_MI_MA];
    printf("===== 登录 =====\n");
    printf("用户名："); scanf("%s", yhm);
    printf("密码：");
    shu_ru_mi_ma(mm, ZUI_DA_MI_MA); 

    for (int i = 0; i < xtsj->yong_hu_shu; i++) {
        if (strcmp(xtsj->yong_hu_shu_zu[i].yong_hu_ming, yhm) == 0) {
            if (strcmp(xtsj->yong_hu_shu_zu[i].mi_ma, mm) == 0) {
                xtsj->dang_qian_yong_hu = i;
                jia_zai_ji_lu_er_jin_zhi(xtsj);
                printf("登录成功！\n");
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

void yong_hu_cai_dan(XiTongShuJu *xtsj) {
    int xuan_ze;
    while (1) {
        printf("\n1. 添加消费记录\n2. 查询记录\n3. 返回主菜单\n选择：");
        scanf("%d", &xuan_ze);
        switch (xuan_ze) {
            case 1: tian_jia_ji_lu(xtsj); break;
            case 2: cha_xun_ji_lu(xtsj); break;
            case 3:
                xtsj->dang_qian_yong_hu = -1;
                xtsj->ji_lu_shu = 0;
                return;
            default:
                printf("输入错误\n");
        }
    }
}

int main() {
    XiTongShuJu xtsj = {0};
    jia_zai_yong_hu_er_jin_zhi(&xtsj);
    int xuan_ze;
    while (1) {
        xian_shi_cai_dan();
        scanf("%d", &xuan_ze);
        switch (xuan_ze) {
            case 1: zhu_ce_yong_hu(&xtsj); break;
            case 2:
                if (deng_lu_yong_hu(&xtsj)) yong_hu_cai_dan(&xtsj);
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
