{                                                               }
{ Mini VGA Paint Program - Public Domain 1992 by Darren Lyon    }
{                                                               }
{  Simple to use: Completely mouse controlled                   }
{                                                               }
{     Use the left button to draw in foreground, right button   }
{     to draw in background. Middle button selects a new        }
{     colour for each button (click the middle button, and      }
{     then select foreground or background by pressing that     }
{     button. Middle button aborts the colour selection. Hit    }
{     any key to quit.                                          }
{                                                               }
{ Requires: VGA, 3-Button mouse, and Turbo Pascal 6             }
{                                                               }
Program VGADraw;

uses crt;

var savearray : array[0..7999] of byte;
    Colour, BackColour : byte;
    Mouse : record
            X, Y : word;
            Buttons : word;
            end;
    Loop : integer;

{ Draw a Pixel at (X,Y) in Colour, in 256 colour mode }
Procedure Putpixel(X, Y : Word; Colour : Byte);
Begin
   asm
     mov ax, $A000
     mov es, ax               { Graphics Segment }
     mov ax, Y
     mov dx, ax               { Load Y loc into DX and AX }
     xchg ah, al              { AH := 256 * Y }
     mov cl, 6
     shl dx, cl               { DX := 64 * Y }
     add ax, dx               { AX := 320 * Y }
     mov dx, X
     add ax, dx               { AX := 320 * Y + X }
     mov di, ax               { ES:DI := address of pixel }
     mov ah, Colour           { Load colour }
     mov es:[di], ah;         { Draw pixel }
   end;
end;

{ Initialize the mouse driver }
Procedure InitMouse;
begin
   asm
     mov ax, 0                { Function 0 }
     int $33
   end;
end;

{ Show the mouse cursor on the screen }
Procedure ShowMouse;
begin
   asm
     mov ax, 1                { Function 1 }
     int $33
   end;
end;

{ Remove the mouse cursor from the screen }
Procedure HideMouse;
begin
   asm
     mov ax, 2                { Function 2 }
     int $33
   end;
end;

{ Return the press state of the mouse buttons }
Procedure MouseButtons;
begin
   asm
     mov ax, 5                { Function 5 }
     mov bx, 0                { All buttons }
     int $33
     mov mouse.buttons, ax
   end;
end;

{ Find the location of the mouse }
Procedure MouseWhere;
begin
   asm
     mov ax, 3                { Function 3 }
     int $33
     mov mouse.x, cx
     mov mouse.y, dx
   end;
end;

{ Place the VGA into 320x200 256 colour mode }
Procedure InitGraphics;
begin
   asm
     mov ax, $13              { Mode 13h }
     int $10
   end;
end;

{ Place the VGA into normal text mode }
Procedure CloseGraphics;
begin
   asm
     mov ax, $3               { Mode 3h }
     int $10
   end;
end;

{ Read the value of the pixel at offset LOC }
Procedure GetPel(Loc : word; var Colour : byte);
begin
   asm
     mov ax, $A000
     mov es, ax               { Graphics segment }
     mov di, Loc              { ES:DI points to pixel }
     mov ah, es:[di]          { Load pixel }
     lds si, colour           { Load variable address }
     mov ds:[si], ah          { Place into variable }
   end;
end;

{ Place a pixel on the screen at location LOC in Colour }
Procedure PutPel(Loc : word; Colour : byte);
begin
   asm
     mov ax, $A000
     mov es, ax               { Graphics segment }
     mov di, Loc              { ES:DI points to pixel }
     mov ah, colour           { Load the colour }
     mov es:[di], ah          { Put the pixel on the screen }
   end;
end;

{ Limit the area the mouse can move to }
Procedure LimitMouse(TopX, TopY, BotX, BotY : word);
begin
   asm
     mov ax,7                 { Function 7 - set X limits }
     mov cx, topx
     mov dx, botx
     int $33
     mov ax,8                 { Function 8 - set Y limits }
     mov cx,topy
     mov dx,boty
     int $33
   end;
end;

{ Process the selection of a new colour }
Procedure NewColour;
var loop1, loop2, loop3, loop4: integer;
begin
   LimitMouse(0, 0, 319, 19);

   { Read the top lines from the screen }
   HideMouse;
   for loop := 0 to 7999 do
       getpel(loop, savearray[loop]);

   { Draw the multi-colour boxes }
   for loop1:= 0 to 3 do
      for loop2:= 0 to 63 do
         for loop3:= 0 to 4 do
            for loop4:= 0 to 4 do
               putpixel(loop2*5+loop3, loop1*5+loop4, loop1*64+loop2);
   showmouse;

   { Get the mouse button press }
   mouse.buttons := 0;
   while (mouse.buttons <> 4) and (mouse.buttons <> 2) and
         (mouse.buttons <> 1) do
      mousebuttons;

   { Find out where they pressed the button }
   mousewhere;

   { Process each button }
   if mouse.buttons = 1 then
      colour := (mouse.y) div 5 * 64 + mouse.x div 5
   else if mouse.buttons = 2 then
      backcolour := (mouse.y) div 5 * 64 + mouse.x div 5;

   { Draw the top portion back on the screen }
   hidemouse;
   for loop := 0 to 7999 do
       putpel(loop, savearray[loop]);
   showmouse;
   limitmouse(0, 0, 319, 199);
end;

{ Mainline begin }
begin
   { Set up the screen, mouse, and colours }
   InitGraphics;
   InitMouse;
   LimitMouse(0, 0, 319, 199);
   ShowMouse;
   Colour := 15;
   BackColour := 0;

   { Keep going until... }
   repeat
   MouseButtons;
   if Mouse.Buttons = 1 then
      begin
      mousewhere;
      putpixel(mouse.x, mouse.y, colour);
      end
   else if Mouse.Buttons = 2 then
      begin
      mousewhere;
      putpixel( mouse.x, mouse.y, backcolour);
      end
   else if mouse.buttons = 4 then
      begin
      newcolour;
      end;
   until keypressed;      { .... a key is pressed }

   CloseGraphics;
end.
