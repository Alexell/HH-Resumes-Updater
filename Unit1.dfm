object Form1: TForm1
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Form1'
  ClientHeight = 647
  ClientWidth = 434
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Visible = True
  PixelsPerInch = 96
  TextHeight = 13
  object HelpLabel: TLabel
    Left = 8
    Top = 606
    Width = 411
    Height = 13
    Caption = 
      #1040#1074#1090#1086#1088#1080#1079#1091#1081#1090#1077#1089#1100' '#1085#1072' '#1089#1072#1081#1090#1077' '#1074' '#1084#1080#1085#1080'-'#1073#1088#1072#1091#1079#1077#1088#1077', '#1087#1086#1089#1083#1077' '#1095#1077#1075#1086' '#1087#1088#1086#1075#1088#1072#1084#1084#1072' '#1085#1072#1095 +
      #1085#1077#1090' '#1088#1072#1073#1086#1090#1091'.'
  end
  object Web: TCppWebBrowser
    Left = 0
    Top = 0
    Width = 434
    Height = 600
    Align = alTop
    TabOrder = 0
    ExplicitLeft = 8
    ExplicitTop = 8
    ExplicitWidth = 414
    ControlData = {
      4C000000DB2C0000033E00000000000000000000000000000000000000000000
      000000004C000000000000000000000001000000E0D057007335CF11AE690800
      2B2E126208000000000000004C0000000114020000000000C000000000000046
      8000000000000000000000000000000000000000000000000000000000000000
      00000000000000000100000000000000000000000000000000000000}
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 625
    Width = 434
    Height = 22
    DoubleBuffered = False
    Panels = <>
    ParentDoubleBuffered = False
    SimplePanel = True
  end
  object StartTimer: TTimer
    Enabled = False
    Interval = 3000
    OnTimer = StartTimerTimer
    Left = 392
    Top = 8
  end
  object MainTimer: TTimer
    Enabled = False
    Interval = 3000
    OnTimer = MainTimerTimer
    Left = 392
    Top = 56
  end
  object LongTimer: TTimer
    Enabled = False
    Interval = 1920000
    OnTimer = LongTimerTimer
    Left = 392
    Top = 104
  end
end
