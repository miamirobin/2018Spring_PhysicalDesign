1. 學號：r06921048
2. 姓名：李友岐
3. 使用之程式語言：< C++ >
4. 使用之編譯器：< GNU g++ >
5. 檔案壓縮方式: zip
6. 各檔案說明：
	 (1) r06921048-p1/src/ : source codes
	 (2) r06921048-p1/src/fm : executable binary
	 (3) r06921048-p1/readme.txt : the text readme file
	 (4) r06921048-p1/report.doc : the report of my program
         (5) r06921048-p1/report.pdf : 附上pdf以避免word格式跑掉

         (6) r06921048-p1/checker/   : To verify the correctness of output result
         (7) r06921048-p1/input/     : The provided test cases
	 (8) r06921048-p1/output/    : The output result

7. 編譯方式說明：        	
   主程式： To compile the code, please type 'make'
            
           To clean the executable binary, please type 'make clean'
    
8. 執行、使用方式說明：
   主程式：
   	編譯完成後，在src下會產生一個名叫 fm 的執行檔
   	執行檔的命令格式為：
   	./fm <input file name> <output file name>
   
   例如：
	要測試 input_0.dat 這筆測資的話
   	則在命令提示下鍵入
   	./fm input_0.dat input_0.out

   驗證程式：位於r06921048-p1/checker/ ，執行檔名為checker_linux，命令格式與主程式相同
                  
9. 執行結果說明（說明執行結果的觀看方法，及解釋各項數據等）：
   主程式：
	 主程式執行時會輸出
      		(1) 目前執行到第幾次Iteration
		(2) Cell的數量
		(3) Cut Size、
	        (4) A組的Cell個數
                (5) B組的Cell個數(假設分成AB兩組)
         以及程式總共的執行時間和記憶體使用量。
	 輸出檔在r06921048-p1/output/
	 
	 例如：執行 ./fm input_1.dat input_1.out 後螢幕會輸出
	    
 	 Parsing Success!                                                 -- 表示讀取資料成功
         Ieration: 7 CellSize:3000 CutSize:1355 SizeA:1496 SizeB:1504     -- (1)(2)(3)(4)(5)
         Total time used  : 0.12 seconds                                  -- 程式總執行時間
         Total memory used: 0.9766 M Bytes                                -- 記憶體用量

	
	
	


