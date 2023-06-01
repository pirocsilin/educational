<html>
  <head>
    <meta http-equiv="content-type" content="text/html; charset=utf-8"/>
    <title></title>
    <style>
      #field{
        width: 40px;
      }
      .step{
        margin-left: 40px;
        margin-top: 40px;
      }
    </style>
  </head>
  <body>
  
  <?php
    function Poisk($n, $k, $i){
      global $Arr, $num, $count;
      if ($n<0) return; 						/*��������� ����������� ������� 
												��� ��� ��������� ��������� �� ������
												� ��������� ���������� */
      
      if ($n==0){      							/*������� ��� ��������� � ���������
												����������. ������� ��������� � 
												������� ����������� ������� �����������*/
                        
        print "<tr><td>$count</td><td>$num = ";
        for($j=0;$j<$i;$j++){
          print "$Arr[$j]";
          if($j<$i-1)
            print " + ";
        }
        print "</tr></td>";
        $count++;
      }
      else{
        if($n - $k >= 0){   				//���� ������� �����, �� ����� ��������� ���������.
          $Arr[$i] = $k;    				//�������� ��� � �������������� ������.
          
          Poisk($n - $k, $k, $i+1); 		//��������� $n �� ��������� ��������� � ��������� 
        }                           		//����� ����� ��� ������������� ��������.
        
        if($k - 1 > 0){     				/*���� ����� $� ��� ����� ������������ � ��������
											����������� (�>0), �� ��������� ��� � ����������
											�������� �� ���������� � ������� �� $n*/
          Poisk($n, $k - 1, $i);
        }                    				//����� ������� ��� ����������.
      }
      return;
    }

    print "<div class='step'>";
    if($_GET['n'] <> ""){
      $n = (integer)$_GET['n'];
      if($n>0 && $n<=20){
        $count = 1;
        $Arr = array();
        $num = $n;
        print "<table border=1 cellpadding=5 cellspacing=0 bordercolor='Gray'>";
        print "<caption><b>���������� ����� $num</b></caption>";
        Poisk($n, $n, 0);
        print "</table>";
      }
      else{
        print "<p>����� �� ������ ������ ����� (n=$_GET[n])";
        print "<ul><li>������� ����� ���  1 <= n <= 20</li></ul>";
      }
    }
  ?>
	
  <form action="" method="GET">
    <p></p>������� ����� <b>n</b> (min 1, max 20):
    &nbsp;&nbsp;<input type="text" name="n" id="field">
    <p><input type="submit" value="����� ����������">
  </form>
  </div>
  
  </body>
</html>