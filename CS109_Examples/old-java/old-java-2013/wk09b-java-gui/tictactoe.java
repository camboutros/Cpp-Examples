// $Id: tictactoe.java,v 1.1 2012-05-31 22:20:30-07 - - $

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import static java.lang.System.*;

class tictactoe {

   static class mouse implements MouseListener {
      Color defaultForeground = Color.BLACK;
      Color defaultBackground = Color.WHITE;

      public void mouseClicked (MouseEvent event) {
         int which = event.getButton();
         JButton button = (JButton) event.getComponent();
         switch (which) {
            case MouseEvent.BUTTON1: button.setText ("X"); break;
            case MouseEvent.BUTTON2: button.setText ("O"); break;
            case MouseEvent.BUTTON3: button.setText (" "); break;
         }
      }

      public void mouseEntered (MouseEvent event) {
         JButton button = (JButton) event.getComponent();
         defaultForeground = button.getForeground();
         defaultBackground = button.getBackground();
         button.setForeground (Color.BLUE);
         button.setBackground (Color.RED);
      }

      public void mouseExited (MouseEvent event) {
         JButton button = (JButton) event.getComponent();
         button.setForeground (defaultForeground);
         button.setBackground (defaultBackground);
      }

      public void mousePressed (MouseEvent event) {
         Toolkit.getDefaultToolkit().beep();
      }

      public void mouseReleased (MouseEvent event) {
         Toolkit.getDefaultToolkit().beep();
      }

   }


   public static void main (String[] args) {
      JFrame frame = new JFrame ("Tic Tac Toe");
      Container pane = frame.getContentPane();
      Font xo_font = new Font ("Monospaced", Font.BOLD, 128);
      JPanel buttons = new JPanel();
      buttons.setLayout (new GridLayout (3, 3));
      for (int index = 0; index < 3; ++index) {
         for (int jndex = 0; jndex < 3; ++jndex) {
            JButton button = new JButton ("");
            button.setFont (xo_font);
            button.setText (" ");
            button.addMouseListener (new mouse());
            buttons.add (button);
         }
      }
      pane.add (buttons);
      frame.pack();
      frame.setLocation (64, 64);
      frame.setVisible (true);
   }
}

// Primary colors:
//   0   0   0 black
//   0   0 255 blue
//   0 255   0 green
//   0 255 255 cyan
// 255   0   0 red
// 255   0 255 magenta
// 255 255   0 yellow
// 255 255 255 white

