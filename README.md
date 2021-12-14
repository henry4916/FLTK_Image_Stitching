# FLTK_Image_Stitching
## Computational-Photography_HW3
### 環境設定：FLTK_1.3.5 opencv_3.1.16
### 作業內容：
#### 1. 建立一個GUI介面，能夠顯示Load進來的圖片、影像拼接後的結果、Text panel以及可供操作的按鈕
	* GUI介面：包含顯示讀取圖片、顯示執行結果、功能按鈕的配置、Text panel配置。
![image](https://user-images.githubusercontent.com/72666141/146067330-2a6c44c5-90c2-4ba4-8756-0cb35ec16057.png)

#### 2. 按鈕功能load Input Image、Image Stitching、Save Output Image、Quit、Reset：
	* load Input Image：按下load按鈕，可以選擇檔案並讀取進使用者介面中，讀取的圖片能夠依序放到介面上(或按下CTRL + F)
	* Image Stitching：按下merge按鈕，將讀取的所有圖片，經過演算法計算，將照片合成一張大圖片，結果需要顯示在gui介面中((或按下CTRL + M)
	* Save Output Image：按下save按鈕，儲存經過拼接後的圖片(或按下CTRL + S)
	* Quit：按下quit按鈕，退出程式(或按下CTRL + Q)
	* Reset：按下reset按鈕重置(或按下CTRL + R)
	* Cut：按下cut按鈕為選取ROI部分，需要先調整滑動條看要原圖多少比例需求(或按下CTRL + C)

https://user-images.githubusercontent.com/72666141/146067745-4387c51a-a45b-4cbb-a979-1c8e943e5c9b.mp4


#### 3. 添加menu功能實現第二點全部的按鈕，且包括快捷鍵
	* CTRL + F -> 選擇檔案
	* CTRL + C -> 選擇ROI部分
	* CTRL + R -> 重置
	* CTRL + M -> 將讀取的所有圖片，經過演算法計算，將照片合成一張大圖片，結果需要顯示在gui介面中
	* CTRL + S -> 儲存經過拼接後的圖片
	* CTRL + Q -> 退出程式

#### 4. Save Output Image：儲存圖片，可以通過修改路徑，而不是單純儲存固定名稱到指定路徑

#### 5. Text panel：Text panel的功能顯示提示，並將該步驟的耗時顯示在畫面上。
