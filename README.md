# HW4_FLTK_Image_Stitching
HW4_FLTK_Image_Stitching

指定作業內容：
	1.建立一個GUI介面，能夠顯示Load進來的圖片、影像拼接後的結果、Text panel以及可供操作的按鈕
		GUI介面：包含顯示讀取圖片、顯示執行結果、功能按鈕的配置、Text panel配置。

	2.按鈕功能load Input Image、Image Stitching、Save Output Image、Quit、Reset：
		a.load Input Image：按下load按鈕，可以選擇檔案並讀取進使用者介面中，讀取的圖片能夠依序放到介面上(或按下CTRL + F)
		b.Image Stitching：按下merge按鈕，將讀取的所有圖片，經過演算法計算，將照片合成一張大圖片，結果需要顯示在gui介面中((或按下CTRL + M)
		c.Save Output Image：按下save按鈕，儲存經過拼接後的圖片(或按下CTRL + S)
		d.Quit：按下quit按鈕，退出程式(或按下CTRL + Q)
		e.Reset：按下reset按鈕重置(或按下CTRL + R)
		f.Cut：按下cut按鈕為選取ROI部分，需要先調整滑動條看要原圖多少比例需求(或按下CTRL + C)
		(另外在GUI最上面有添加menu，包含以上全部內容）

	3.Image Stitching：影像拼接的功能，可拼接的圖片數量及最終效果。
		Image Stitching：在處理ROI部分為自己實做，內容包含如何通過滑動條選取所需比例對圖片進行分割，且顯示所分割出的圖片在GUI中(第二層box中按下cut按鈕後有不同顏色框起來的部分)，讓使用者好觀察，另外不同圖片數量合成時可以選擇不同的比例，功能為減少合成時間，另外部分使用opencv3.4.16的Stitching函式(因為考慮到最終結果會是評量標準，而自己實做的部分效果較不佳），影片2是為了說明ROI的功能正常，選取的比例會影響演算法處理的時間（選取100%與50%進行比較，合成時間減少大約一半）

	4.Save Output Image：儲存圖片，可以通過修改路徑，而不是單純儲存固定名稱到指定路徑

	5.Text panel：Text panel的功能顯示提示，並將該步驟的耗時顯示在畫面上。

Bonus：
	1.介面美化
		a.加入白底讓load的圖片顯示，不讓畫面太突兀，調整好比例剛剛好顯示在box內
		b.調整每個按鈕與顯示的格子距離與大小，使界面美觀
	2.儲存檔案可以像另存新檔的方式，不是單純儲存固定名稱到指定路徑(1.demo影片23秒)
	3.自行添加額外的拼接圖片數量5~10張拼接
		而外添加大概10張，自己剪的圖片來當作資源，觀察結果
	4.同學其他的創意
		a.添加可以通過滑動條動態調整ROI部分，功能可以減少合成時間
		b.在界面最上面添加而外的menu功能，讓使用者處理操作button外，也可以使用menu來完成所需操作(1.demo影片10秒)
		c.添加快捷鍵功能(1.demo影片1分22秒)
			* CTRL + F -> 選擇檔案
			* CTRL + C -> 選擇ROI部分
			* CTRL + R -> 重置
			* CTRL + M -> 將讀取的所有圖片，經過演算法計算，將照片合成一張大圖片，結果需要顯示在gui介面中
			* CTRL + S -> 儲存經過拼接後的圖片
			* CTRL + Q -> 退出程式
