import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

# header_dir="./graph/"
# file_name="graph.png"

# 第一引数(グラフを作成したいCSVファイルのパス)を文字列に変換する。

dir=str(input("input directory name: "))
file_name=str(input("file_name: "))

file=dir+"/"+file_name

# # file pathからファイル名だけを取り出す

# # ファイル名（拡張子あり）
# file_name_with_extension = os.path.basename(file_path)

# # 拡張子を除いたファイル名
# file_name_without_extension, extension = os.path.splitext(file_name_with_extension)

# file名の表示
print(file+" has readed.")

# CSVファイルをPandasのSeriesに読み込む　
y_values= pd.read_csv(file,header=None)

x_values=range(len(y_values));


# print(str(len(df))+" datum were found.")

# start = int(input("input start datum number."))
# end = int(input("input end datum number."))

# df=df.iloc[start:end];

# print(y_values)
plt.plot(x_values, y_values, linestyle='-', color='b')

# plt.plot(data.index, data[0], linestyle='-', color='b')

# グラフのラベルやタイトルの設定
plt.xlabel('x')  # X軸のラベル
plt.ylabel('data')  # Y軸のラベル
plt.title('plot of csv data')  # グラフタイトル
plt.grid(True)  # グリッドを表示

# グラフを表示
plt.show()



# 折れ線グラフを作成して表示
# plt.plot(df["x"] , df["data"], linestyle="solid")

# # グラフにタイトルや軸ラベルを追加（必要に応じて）
# plt.title('title-none')

# # x軸ラベルの設定
# plt.xlabel('x-axis')

# # y軸ラベルの設定
# plt.ylabel('y-axis')

# # plt.show()に後にsaveすると消えてしまう。
# plt.savefig(header_dir+file_name_without_extension+".png")

# print(str(header_dir)+str(file_name_without_extension)+".png was saved..")

# グラフを表示
# plt.show()

# メモリの解放
plt.clf()
plt.close()
