# NX-Solver
Numerical Expression Solver (NX-Script)

A scripting language for computing numerical expressions on the fly. 

Sample codes:

' Compute Distance in 3d;
w = 10;
x = 20;
y = 30;
z = $sqrt(w * w + x * x + y * y);
$prt(z);


' Lerping
x1 = 100;
x2 = -200;
width = x2 - x1;
diff = 10;

i = 0;

$for(1, 10, 1, 
        
        $bl
        (
            le = (x + diff) / width,
            
        )
    );
        