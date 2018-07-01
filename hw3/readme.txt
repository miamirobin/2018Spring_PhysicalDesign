1. 學號：r06921048
2. 姓名：李友岐
3. 使用之程式語言：< C++ >
4. 使用之編譯器：< GNU g++ >
5. 檔案壓縮方式: zip
6. 各檔案說明：
	 (1) r06921048-p3/src/ : source codes
	 (2) r06921048-p3/place : executable binary
	 (3) r06921048-p3/readme.txt : the text readme file
	 (4) r06921048-p3/report.doc : the report of my program
                 

7. 編譯方式說明：        	
   主程式： To compile the code, please type 'make'
            
           To clean the executable binary, please type 'make clean'
    
8. 執行、使用方式說明：
   主程式：
   	編譯完成後，在src下會產生一個名叫 place 的執行檔
   	執行檔的命令格式為：
   	./place -aux inputFile.aux 
   
   例如：
	要測試 ibm01 這筆測資的話
   	則在命令提示下鍵入
        cd src
   	./place -aux ibm01-cu85.aux 


  
                  
9. 執行結果說明（說明執行結果的觀看方法，及解釋各項數據等）：
   主程式：
	 主程式執行完會輸出
      		(1) Global Placement結束後的HPWL, Global Placement花費的時間。
		(2) Legalization結束後的HPWL, Legalization花費的時間。
		(3) Detail Placement結束後的HPWL, Detail Placement花費的時間。
                (4) 最終的HPWL, 總共花費的時間。
	       
               
	 
	 
	 例如：執行 ./place -aux ibm01-cu85.aux  後螢幕會輸出
	    
 	 Global HPWL: 48604486   Time:   96.0 sec (1.6 min)           --(1)  
          Legal HPWL: 86787028   Time:    0.0 sec (0.0 min)           --(2)
         Detail HPWL: 67436793   Time:    1.0 sec (0.0 min)           --(3)
 ===================================================================
                HPWL: 67436793   Time:   97.0 sec (1.6 min)           --(4)

       
	
	
	


