#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <random>
#include <string.h>      //文字列の代入に使う
#include <bits/stdc++.h> //piの利用で必要(M_PI)

class ParticleInfo
{
public:
    uint32_t id;
    uint32_t type;
    /*position*/
    double posx;
    double posy;
    double posz;
    /*velocity*/
    double velx;
    double vely;
    double velz;
    /*結合*/
    uint32_t bond_pair[2];
    uint32_t bond_type[2];
    uint32_t nbond = 0;
    /*アングル*/
    uint32_t angle_pair[2][3];
    uint32_t angle_type[2];
    uint32_t nangle = 0;
    //ベシクル内の水かどうかwater1,2
    bool vesicle_water1 = false;
    bool vesicle_water2 = false;

    //sortを利用するために定義
    bool operator<(const ParticleInfo &another) const
    {
        //メンバ変数であるnum1で比較した結果を
        //この構造体の比較とする
        return id < another.id;
    }
};

class LipidInfo
{
public:
    uint32_t id;
    uint32_t type;
    /*position*/
    double posx;
    double posy;
    double posz;
    //sortを利用するために定義
    bool operator<(const LipidInfo &another) const
    {
        //メンバ変数であるnum1で比較した結果を
        //この構造体の比較とする
        return id < another.id;
    }
};

class CenterOfGravity
{
public:
    /*position*/
    double x;
    double y;
    double z;
    /*数えた脂質粒子の個数*/
    int num;
    CenterOfGravity()
    {
        x = y = z = 0;
        num = 0;
    }
};

int main(int argc, char *argv[])
{
    std::vector<ParticleInfo> before_pinfo, after_pinfo;
    std::vector<LipidInfo> lipidinfo1, lipidinfo2;
    ParticleInfo temp_info;
    LipidInfo temp_lipidinfo;
    //------------------------------------------------------//
    //
    //
    //計算前の座標の読み込みを行う．
    //
    //
    //
    //------------------------------------------------------//
    std::ifstream ifs0(argv[1]);
    if (!ifs0)
    {
        std::cerr << "errorifs0" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    //いらないはじめの3行を捨てる．
    std::string delete_str[3];
    for (uint32_t i = 0; i < 3; i++)
    {
        std::getline(ifs0, delete_str[i]);
    }
    //ファイルの読み込み　粒子番号,粒子種は0から開始する．
    while (ifs0 >> temp_info.id >> temp_info.type >> temp_info.posx >> temp_info.posy >> temp_info.posz)
    {
        temp_info.id--;
        temp_info.type--;
        before_pinfo.push_back(temp_info);
        if (temp_info.type == 1)
        {
            temp_lipidinfo.posx = temp_info.posx;
            temp_lipidinfo.posy = temp_info.posy;
            temp_lipidinfo.posz = temp_info.posz;
            lipidinfo1.push_back(temp_lipidinfo);
        }
        if (temp_info.type == 4)
        {
            temp_lipidinfo.posx = temp_info.posx;
            temp_lipidinfo.posy = temp_info.posy;
            temp_lipidinfo.posz = temp_info.posz;
            lipidinfo2.push_back(temp_lipidinfo);
        }
    }
    ifs0.close();
    //はじめの文字列を読み込む
    double box_sx, box_sy, box_sz, box_ex, box_ey, box_ez, box_wt;
    sscanf(delete_str[0].c_str(), "'box_sx=%lf box_sy=%lf box_sz=%lf box_ex=%lf box_ey=%lf box_ez=%lf box_wt=%lf",
           &box_sx, &box_sy, &box_sz, &box_ex, &box_ey, &box_ez, &box_wt);
    //    std::cout <<std::setprecision(10)<< box_sx << " " << box_sy << " " << box_sz << " " << box_ex << " " << box_ey << " " << box_ez << " " << box_wt << std::endl;
    std::sort(before_pinfo.begin(), before_pinfo.end()); //classでオペレータを定義して利用している．
    std::sort(lipidinfo1.begin(), lipidinfo1.end());     //classでオペレータを定義して利用している．
    std::sort(lipidinfo2.begin(), lipidinfo2.end());     //classでオペレータを定義して利用している．
    //------------------------------------------------------//
    //
    //
    //計算後の座標の読み込みを行う．
    //
    //
    //
    //------------------------------------------------------//
    std::ifstream ifs1(argv[2]);
    if (!ifs1)
    {
        std::cerr << "errorifs1" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    //いらないはじめの3行を捨てる．
    std::string delete_str1[3];
    for (uint32_t i = 0; i < 3; i++)
    {
        std::getline(ifs1, delete_str1[i]);
    }
    //ファイルの読み込み　粒子番号,粒子種は0から開始する．
    while (ifs1 >> temp_info.id >> temp_info.type >> temp_info.posx >> temp_info.posy >> temp_info.posz)
    {
        temp_info.id--;
        temp_info.type--;
        after_pinfo.push_back(temp_info);
    }
    ifs1.close();
    //はじめの文字列を読み込む
    double box_sx1, box_sy1, box_sz1, box_ex1, box_ey1, box_ez1, box_wt1;
    sscanf(delete_str[0].c_str(), "'box_sx=%lf box_sy=%lf box_sz=%lf box_ex=%lf box_ey=%lf box_ez=%lf box_wt=%lf",
           &box_sx1, &box_sy1, &box_sz1, &box_ex1, &box_ey1, &box_ez1, &box_wt1);
    //    std::cout <<std::setprecision(10)<< box_sx << " " << box_sy << " " << box_sz << " " << box_ex << " " << box_ey << " " << box_ez << " " << box_wt << std::endl;
    std::sort(after_pinfo.begin(), after_pinfo.end()); //classでオペレータを定義して利用している．
    //------------------------------------------------------//
    //
    //
    //ベシクル1の重心を計算する．
    //
    //
    //
    //------------------------------------------------------//
    CenterOfGravity center_vesicle1, center_vesicle2;
    for (int i = 0; i < before_pinfo.size(); i++)
    {
        if (before_pinfo.at(i).type == 1)
        {
            center_vesicle1.x += before_pinfo.at(i).posx;
            center_vesicle1.y += before_pinfo.at(i).posy;
            center_vesicle1.z += before_pinfo.at(i).posz;
            center_vesicle1.num++;
        }
    }
    center_vesicle1.x /= (double)center_vesicle1.num;
    center_vesicle1.y /= (double)center_vesicle1.num;
    center_vesicle1.z /= (double)center_vesicle1.num;
    std::cout << center_vesicle1.x << " " << center_vesicle1.y << " " << center_vesicle1.z << std::endl;
    //------------------------------------------------------//
    //
    //
    //ベシクル2の重心を計算する．
    //
    //
    //
    //------------------------------------------------------//
    for (int i = 0; i < before_pinfo.size(); i++)
    {
        if (before_pinfo.at(i).type == 4)
        {
            center_vesicle2.x += before_pinfo.at(i).posx;
            center_vesicle2.y += before_pinfo.at(i).posy;
            center_vesicle2.z += before_pinfo.at(i).posz;
            center_vesicle2.num++;
        }
    }
    center_vesicle2.x /= (double)center_vesicle2.num;
    center_vesicle2.y /= (double)center_vesicle2.num;
    center_vesicle2.z /= (double)center_vesicle2.num;
    std::cout << center_vesicle2.x << " " << center_vesicle2.y << " " << center_vesicle2.z << std::endl;
    //------------------------------------------------------//
    //
    //
    //重心間距離を元に，ベシクル半径を計算し，水粒子にラベル付する．
    //
    //
    //
    //------------------------------------------------------//
    //radius of vesicles重心間距離/2-bond長さかける3
    /*double vesicle_radius = sqrt(pow(center_vesicle1.x - center_vesicle2.x, 2) + pow(center_vesicle1.y - center_vesicle2.y, 2) + pow(center_vesicle1.z - center_vesicle2.z, 2)) / 2 - 0.7 * 3;
    for (int i = 0; i < before_pinfo.size(); i++)
    {
        if (sqrt(pow(center_vesicle1.x - before_pinfo.at(i).posx, 2) + pow(center_vesicle1.y - before_pinfo.at(i).posy, 2) + pow(center_vesicle1.z - before_pinfo.at(i).posz, 2)) < vesicle_radius)
        {
            after_pinfo.at(i).type = 5;
        }
        if (sqrt(pow(center_vesicle2.x - before_pinfo.at(i).posx, 2) + pow(center_vesicle2.y - before_pinfo.at(i).posy, 2) + pow(center_vesicle2.z - before_pinfo.at(i).posz, 2)) < vesicle_radius)
        {
            after_pinfo.at(i).type = 6;
        }
    }*/
    //------------------------------------------------------//
    //
    //
    //重心間距離を元に，ベシクル半径を計算し，水粒子にラベル付する．
    //脂質との内積を計算し，正確にラベル付する．
    //
    //
    //------------------------------------------------------//
    //radius of vesicles重心間距離/2
    double vesicle_radius = sqrt(pow(center_vesicle1.x - center_vesicle2.x, 2) + pow(center_vesicle1.y - center_vesicle2.y, 2) + pow(center_vesicle1.z - center_vesicle2.z, 2)) / 2;
    //vesicle_radius_short内は水とラベル．それよりも大きく，vesicle_radius_long内は内積で区別．
    double vesicle_radius_short = vesicle_radius - 0.7 * 9;
    double vesicle_radius_long = vesicle_radius + 0.7 * 9;
    for (int i = 0; i < before_pinfo.size(); i++)
    {
        if (sqrt(pow(center_vesicle1.x - before_pinfo.at(i).posx, 2) + pow(center_vesicle1.y - before_pinfo.at(i).posy, 2) + pow(center_vesicle1.z - before_pinfo.at(i).posz, 2)) < vesicle_radius_short)
        {
            after_pinfo.at(i).type = 5;
        }
        else if (sqrt(pow(center_vesicle1.x - before_pinfo.at(i).posx, 2) + pow(center_vesicle1.y - before_pinfo.at(i).posy, 2) + pow(center_vesicle1.z - before_pinfo.at(i).posz, 2)) < vesicle_radius_long)
        {
            LipidInfo closelipid;
            double min_distance = 10000000000000000;
            double distance;
            uint32_t min_id;
            for (int j = 0; j < lipidinfo1.size(); j++)
            {
                distance = sqrt(pow(lipidinfo1.at(j).posx - before_pinfo.at(i).posx, 2) + pow(lipidinfo1.at(j).posy - before_pinfo.at(i).posy, 2) + pow(lipidinfo1.at(j).posz - before_pinfo.at(i).posz, 2));
                if (distance < min_distance)
                {
                    min_distance = distance;
                    min_id = j;
                }
            }
            if ((lipidinfo1.at(min_id).posx - before_pinfo.at(i).posx) * (before_pinfo.at(i).posx - center_vesicle1.x) + (lipidinfo1.at(min_id).posy - before_pinfo.at(i).posy) * (before_pinfo.at(i).posy - center_vesicle1.y) + (lipidinfo1.at(min_id).posz - before_pinfo.at(i).posz) * (before_pinfo.at(i).posz - center_vesicle1.z) > 0)
            {
                after_pinfo.at(i).type = 5;
            }
        }

        if (sqrt(pow(center_vesicle2.x - before_pinfo.at(i).posx, 2) + pow(center_vesicle2.y - before_pinfo.at(i).posy, 2) + pow(center_vesicle2.z - before_pinfo.at(i).posz, 2)) < vesicle_radius_short)
        {
            after_pinfo.at(i).type = 6;
        }
        else if (sqrt(pow(center_vesicle2.x - before_pinfo.at(i).posx, 2) + pow(center_vesicle2.y - before_pinfo.at(i).posy, 2) + pow(center_vesicle2.z - before_pinfo.at(i).posz, 2)) < vesicle_radius_long)
        {
            LipidInfo closelipid;
            double min_distance = 10000000000000000;
            double distance;
            uint32_t min_id;
            for (int j = 0; j < lipidinfo1.size(); j++)
            {
                distance = sqrt(pow(lipidinfo2.at(j).posx - before_pinfo.at(i).posx, 2) + pow(lipidinfo2.at(j).posy - before_pinfo.at(i).posy, 2) + pow(lipidinfo2.at(j).posz - before_pinfo.at(i).posz, 2));
                if (distance < min_distance)
                {
                    min_distance = distance;
                    min_id = j;
                }
            }
            if ((lipidinfo2.at(min_id).posx - before_pinfo.at(i).posx) * (before_pinfo.at(i).posx - center_vesicle2.x) + (lipidinfo2.at(min_id).posy - before_pinfo.at(i).posy) * (before_pinfo.at(i).posy - center_vesicle2.y) + (lipidinfo2.at(min_id).posz - before_pinfo.at(i).posz) * (before_pinfo.at(i).posz - center_vesicle2.z) > 0)
            {
                after_pinfo.at(i).type = 6;
            }
        }
    }
    //------------------------------------------------------//
    //
    //
    //出力ファイルを生成する．
    //
    //
    //
    //------------------------------------------------------//
    //open file
    //pos_file

    FILE *fpo0;
    fpo0 = fopen(argv[3], "w");
    if (fpo0 == NULL)
    {
        printf("fpo0\n");
        return -1;
    }
    fprintf(fpo0, "'box_sx=%lf box_sy=%lf box_sz=%lf box_ex=%lf box_ey=%lf box_ez=%lf box_wt=%lf\n", box_sx, box_sy, box_sz, box_ex, box_ey, box_ez, box_wt);
    for (int i = 1; i < 3; i++)
    {
        fprintf(fpo0, "%s \n", delete_str[i].c_str());
    }
    for (int i = 0; i < after_pinfo.size(); i++)
    {
        fprintf(fpo0, "%d %d   %lf   %lf   %lf \n",
                after_pinfo.at(i).id + 1,
                after_pinfo.at(i).type + 1,
                after_pinfo.at(i).posx,
                after_pinfo.at(i).posy,
                after_pinfo.at(i).posz);
    }
    fclose(fpo0);

    return 0;
}