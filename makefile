#  Kpro_C++
#Compiler           =   clang++
Compiler		   =   g++-9
Compile_Version    =   -std=gnu++1z
Optimization       =   -O3
Extra_Option	   =   -Wall
Target_Program     =   src/main.cpp
Executable_File    =   src/main
Input_File01       =   input/input01.txt
Result_file01      =   result01.txt
Expect_file01      =   expect/expect01.txt

Writer_Python	   =   input/write.py

# 実行可能ファイル main の作成
All       : $(Executable_File)

$(Executable_File) : $(Target_Program)
	$(Compiler) $(Compile_Version) $(Optimization) $(Extra_Option) -o $(Executable_File) $(Target_Program)

# 実行可能ファイル main の実行
run       :  $(Executable_File)
	./$(Executable_File)

# 実行可能ファイル main に input01.txtを入力
test      : $(Executable_File) $(Input_File01)
	@./$(Executable_File) < $(Input_File01)

# 時間を計測して, 実行可能ファイル main にinput01.txtを入力
test_time : $(Result_file01)
	@time ./$(Executable_File) < $(Input_File01) > $(Result_file01)

judge     :  $(Result_file01)
	@make test_time
	@-diff -q $(Expect_file01) $(Result_file01)
	@make clean_result

writePy		: $(Writer_Python)
	python3 $(Writer_Python)

$(Result_file01) :
	@touch $(Result_file01)

# 使用済みの result01 を消去
clean_result : $(Result_file01)
	@rm -f $(Result_file01)

# 実行可能ファイルを消去
clean     :
	rm -f $(Executable_File)