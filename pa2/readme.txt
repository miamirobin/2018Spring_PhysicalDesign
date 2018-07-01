1. 學號：r06921048
2. 姓名：李友岐
3. 使用之程式語言：< C++ >
4. 使用之編譯器：< GNU g++ >
5. 檔案壓縮方式: zip
6. 各檔案說明：
	 (1) r06921048-p2/src/ : source codes
	 (2) r06921048-p2/src/Floorplan : executable binary
	 (3) r06921048-p2/readme.txt : the text readme file
	 (4) r06921048-p2/report.doc : the report of my program
         (5) r06921048-p2/report.pdf : 附上pdf以避免word格式跑掉

         (6) r06921048-p2/checker/   : To verify the correctness of output result
         (7) r06921048-p2/input/     : The provided test cases
	 (8) r06921048-p2/output/    : The output result

7. 編譯方式說明：        	
   主程式： To compile the code, please type 'make'
            
           To clean the executable binary, please type 'make clean'
    
8. 執行、使用方式說明：
   主程式：
   	編譯完成後，在src下會產生一個名叫 Floorplan 的執行檔
   	執行檔的命令格式為：
   	./Floorplan [α value] [input.block name] [input.net name] [output file name] 
   
   例如：
	要測試 apte 這筆測資的話
   	則在命令提示下鍵入
        cd src
   	./Floorplan 0.5 ../input/apte.block ../input/apte.nets ../output/apte.rpt


   驗證程式：位於r06921048-p2/checker/ ，執行檔名為checker，命令格式為：
								./Floorplan [input.block name] [input.net name] [output file name] [α value]
                  
9. 執行結果說明（說明執行結果的觀看方法，及解釋各項數據等）：
   主程式：
	 主程式執行時會輸出
      		(1) 目前的T
		(2) 目前的Best Cost
		(3) time usage
	        (4) memory usage
               
	 輸出檔在r06921048-p2/output/
	 
	 例如：執行 ./Floorplan 0.5 ../input/xerox.block ../input/xerox.nets ../output/xerox.rpt 後螢幕會輸出
	    
 	 
         T = 0.0010989,   Best Cost= 1.12238e+07               --(1), (2)
         Total time used  : 0.35 seconds                       --(3) 
         Total memory used: 205.4 M Bytes                      --(4)
       

	
	
	


