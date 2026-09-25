#pragma once
#include <QString>

inline QString appStyleSheet() {
    return R"(
        QWidget { 
            font-family: "DejaVu Sans"; 
            font-size: 13px; 
            color: #3d3d3a; 
        }
        QMainWindow, QWidget#page { 
            background: #ffffff; 
        }

        QFrame#titleBar { 
            background: #4285C7; 
        }
        QLabel#titleLabel { 
            color: white; 
            font-size: 15px; 
            font-weight: bold; 
            padding-left: 14px; 
        }

        QFrame#sidebar { 
            background: #FAF9F6; 
            border-right: 1px solid #E4E2D8; 
        }
        QPushButton#menuBtn {
            text-align: left; 
            padding: 10px 16px; 
            border: none; 
            background: transparent;
            border-radius: 6px; 
            margin: 3px 10px; 
            color: #3d3d3a;
        }
        QPushButton#menuBtn:checked { 
            background: #DFEBF7; 
            color: #2C6BA8; 
            font-weight: bold; 
        }
        QPushButton#menuBtn:hover:!checked { 
            background: #EFEFEA; 
        }

        QPushButton#btnPrimary {
            background: #4285C7; 
            color: white; 
            border: none; 
            border-radius: 6px;
            padding: 8px 18px; 
            font-weight: bold;
        }
        QPushButton#btnPrimary:hover { 
            background: #386fa8; 
        }

        QPushButton#btnSuccess {
            background: #438F78; 
            color: white; 
            border: none; 
            border-radius: 6px;
            padding: 8px 18px; 
            font-weight: bold;
        }
        QPushButton#btnSuccess:hover { background: #357a66; }

        QPushButton#btnFlat {
            background: white; 
            color: #3d3d3a; 
            border: 1px solid #96948C; 
            border-radius: 6px;
            padding: 8px 18px;
        }
        QPushButton#btnFlat:hover { 
            background: #F2F1EC; 
        }

        QPushButton#btnDanger {
            background: white; 
            color: #B8503F; 
            border: 1px solid #B8503F; 
            border-radius: 6px;
            padding: 8px 18px;
        }
        QPushButton#btnDanger:hover { 
            background: #FBEDEA; 
        }

        QTableWidget {
            border: 1px solid #96948C; 
            gridline-color: #E4E2D8; 
            background: white;
            selection-background-color: #DFEBF7; 
            selection-color: #2C6BA8;
        }
        QHeaderView::section {
            background: #DBE7F3; 
            color: #2C3E50; 
            font-weight: bold; 
            padding: 6px;
            border: none; 
            border-bottom: 1px solid #96948C;
        }
        QLineEdit, QComboBox, QDateEdit, QSpinBox, QDoubleSpinBox {
            border: 1px solid #96948C; 
            border-radius: 5px; 
            padding: 6px 8px; 
            background: white;
        }
        QLineEdit:focus, QComboBox:focus { 
            border: 1px solid #4285C7; 
        }

        QFrame#card { 
            background: #F2F1EC; 
            border-radius: 8px; 
        }
        QFrame#statCard { background: white; 
            border: 1px solid #E4E2D8; 
            border-radius: 8px; 
        }

        QFrame#successBox { 
            background: #D7EDE8; 
            border: 2px solid #438F78; 
            border-radius: 8px; 
        }
        QFrame#taxiCard { 
            background: white; 
            border: 1px solid #96948C; 
            border-radius: 6px; 
        }
        QFrame#taxiCardSelected { 
            background: #DFEBF7; 
            border: 2px solid #4285C7; 
            border-radius: 6px; 
        }
    )";
}