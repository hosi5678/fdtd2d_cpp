import os
import csv
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import matplotlib.patches as patches

from count_csv_files import count_csv_files
from shell_command import shell_command

# 前処理
command="rm ./pngs/*.png"
shell_command(command)

center_point_file="./csv_files/point.csv"
df=pd.read_csv(center_point_file,header=None)

center_y=df.iloc[0]
half_y=df.iloc[1]

center_x=center_y
half_x=half_y

rect_y=center_y-half_y
rect_x=center_x-half_x

# 最大値、最小値のファイルを開く
ey_range_file="./csv_files/ez_range.csv"
df=pd.read_csv(ey_range_file,header=None)

# 最大値、最小値の表示
print("ez_max="+str(df.iloc[0].max()))
print("ez_min="+str(df.iloc[1].min()))

# 最大値、最小値の取得
vmax=df.iloc[0].max()
vmin=df.iloc[1].min()

vmax=1.0e-7
vmin=-1.0e-7

# csv filesのフォルダパス
csv_dir="./ez_timestep/"

# csvのファイル数の取得
timestep = count_csv_files(csv_dir)

# csv fileのfile数の表示
print(str(timestep)+" csv files were found.")

# 出力するフレーム数の入力
timestep = int(input("input timestep number.(>0)"))

for i in range(timestep):
    
    # 6桁固定のiを作り出す
    fmt_i = '{:06d}'.format(i)
    csv_file=csv_dir+"ez_timestep_"+fmt_i+".csv"

    # data frameに読み込む
    df=pd.read_csv(csv_file,header=0)

    # 先頭のタブ1個だけ削除
    df.columns=df.columns.str.strip()
    # x軸のデータ（1行目）
    x_values=df.columns;
        
    # 2行目以降の1列目をy軸の値として取得
    y_values = df.index[1:]
        
    # 2行目以降のデータを取得
    data = df.iloc[1:, 1:]

    # figure(全体図)のサイズを1080を中心とした正方形に設定
    fig=plt.figure(figsize=(1300/80,1080/80),dpi=80)

    fig.suptitle('Heatmap of Ez',fontsize=20)

    # heatmapにもx軸を設定する。間隔はxticklabels=5で設定する。
    # vmax,vminの設定
    df = df.reset_index()  # MultiIndex を解除
   
    x_values=df.index.astype(int)
    
    y_values=df.index.astype(int)
    
    # y軸の目盛り間隔とラベルを設定
    y_tick_positions = range(0, len(y_values), 5)
    x_tick_positions = range(0, len(x_values), 5)

    heatmap=sns.heatmap(
        data,
        xticklabels=5,
        yticklabels=5,
        cmap='coolwarm',
        cbar=True,
        center=0.0,
        vmax=vmax,
        vmin=vmin
    )

    heatmap.set_yticks(y_tick_positions)
    heatmap.set_yticklabels(y_values[y_tick_positions], fontsize=15)

    heatmap.set_xticks(x_tick_positions)
    heatmap.set_xticklabels(x_values[x_tick_positions], fontsize=15)

    rect=patches.Rectangle((float(rect_x.iloc[0]),float(rect_y.iloc[0])),float(2*half_x.iloc[0]),float(2*half_x.iloc[0]),
    edgecolor="black",facecolor="none",linewidth=2)

    # print(type(rect))

    heatmap.add_patch(rect)

    heatmap.invert_yaxis()

    heatmap.set_xlabel('x position' , {"fontsize":20} )
    heatmap.set_ylabel('y position' , {"fontsize":20} )
    
    plt.suptitle("timestep="+str(i),fontsize=25)
    
    # ファイルを保存
    plt.savefig("./pngs/png_"+fmt_i+".png")
    
    # saveの表示
    if (i%100==0):
        print("./pngs/png_"+fmt_i+".png"+"/"+str(timestep)+" was created.")
    
    # csv fileの削除
    # os.unlink(csv_file)
    
    # メモリの解放
    plt.clf()
    plt.close()

# 後処理
command="rm ./mp4/output.mp4"
shell_command(command)

command="ffmpeg -r 10 -i ./pngs/png_%06d.png -c:v libx264 -pix_fmt yuv420p ./mp4/output.mp4"
shell_command(command)    

command="vlc ./mp4/output.mp4"
shell_command(command)
