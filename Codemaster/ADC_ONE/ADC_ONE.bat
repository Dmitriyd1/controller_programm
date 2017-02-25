REM   C:\Users\Дмитрий\Desktop\Дима\4 курс\2 семестр\Курсовая работа 4курс 2 сем\Codemaster\ADC_ONE\ADC_ONE.ipp - command file
REM   Created: Feb 25, 2017  23:45:12

PushD
CD..
If Exist "C:\Users\Дмитрий\Desktop\Дима\4 курс\2 семестр\Курсовая работа 4курс 2 сем\Codemaster\ADC_ONE\Debug\Exe\ADC_ONE.MCE" Del "C:\Users\Дмитрий\Desktop\Дима\4 курс\2 семестр\Курсовая работа 4курс 2 сем\Codemaster\ADC_ONE\Debug\Exe\ADC_ONE.MCE"
If Exist "C:\Users\Дмитрий\Desktop\Дима\4 курс\2 семестр\Курсовая работа 4курс 2 сем\Codemaster\ADC_ONE\Debug\Exe\ADC_ONE.HEX" Del "C:\Users\Дмитрий\Desktop\Дима\4 курс\2 семестр\Курсовая работа 4курс 2 сем\Codemaster\ADC_ONE\Debug\Exe\ADC_ONE.HEX"
"D:\CodeMaster-96\1_32_00\CMC-96\Bin\MCC96.EXE" @"C:\Users\Дмитрий\Desktop\Дима\4 курс\2 семестр\Курсовая работа 4курс 2 сем\Codemaster\ADC_ONE\ADC_ONE.{1}"
"D:\CodeMaster-96\1_32_00\CMC-96\Bin\MCLINK.EXE" @"C:\Users\Дмитрий\Desktop\Дима\4 курс\2 семестр\Курсовая работа 4курс 2 сем\Codemaster\ADC_ONE\ADC_ONE.(1)"
PopD
