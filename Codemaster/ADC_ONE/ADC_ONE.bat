REM   G:\�������� ������ 4���� 2 ���\Codemaster\ADC_ONE\ADC_ONE.ipp - command file
REM   Created: Feb 28, 2017  15:52:28

PushD
CD..
If Exist "G:\�������� ������ 4���� 2 ���\Codemaster\ADC_ONE\Debug\Exe\ADC_ONE.MCE" Del "G:\�������� ������ 4���� 2 ���\Codemaster\ADC_ONE\Debug\Exe\ADC_ONE.MCE"
If Exist "G:\�������� ������ 4���� 2 ���\Codemaster\ADC_ONE\Debug\Exe\ADC_ONE.HEX" Del "G:\�������� ������ 4���� 2 ���\Codemaster\ADC_ONE\Debug\Exe\ADC_ONE.HEX"
"C:\Program Files (x86)\CodeMaster-96\1_32_00\CMC-96\Bin\MCC96.EXE" @"G:\�������� ������ 4���� 2 ���\Codemaster\ADC_ONE\ADC_ONE.{1}"
"C:\Program Files (x86)\CodeMaster-96\1_32_00\CMC-96\Bin\MCLINK.EXE" @"G:\�������� ������ 4���� 2 ���\Codemaster\ADC_ONE\ADC_ONE.(1)"
PopD
