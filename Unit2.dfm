object SettingsForm: TSettingsForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
  ClientHeight = 177
  ClientWidth = 332
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Position = poScreenCenter
  OnShow = FormShow
  TextHeight = 13
  object CountryNote: TLabel
    Left = 15
    Top = 130
    Width = 307
    Height = 39
    Caption = 
      #1054#1073#1088#1072#1090#1080#1090#1077' '#1074#1085#1080#1084#1072#1085#1080#1077', '#1095#1090#1086' '#1074#1085#1077' '#1079#1072#1074#1080#1089#1080#1084#1086#1089#1090#1080' '#1086#1090' '#1074#1099#1073#1088#1072#1085#1085#1086#1081' '#1089#1090#1088#1072#1085#1099', '#1089#1072#1081#1090 +
      ' '#1074' '#1087#1088#1086#1075#1088#1072#1084#1084#1077' '#1073#1091#1076#1077#1090' '#1086#1090#1086#1073#1088#1072#1078#1072#1090#1100#1089#1103' '#1085#1072' '#1088#1091#1089#1089#1082#1086#1084' '#1103#1079#1099#1082#1077', '#1101#1090#1086' '#1085#1077#1086#1073#1093#1086#1076#1080#1084#1086 +
      ' '#1076#1083#1103' '#1082#1086#1088#1088#1077#1082#1090#1085#1086#1081' '#1088#1072#1073#1086#1090#1099' '#1087#1088#1086#1075#1088#1072#1084#1084#1099'.'
    WordWrap = True
  end
  object CountryLabel: TLabel
    Left = 10
    Top = 10
    Width = 45
    Height = 13
    Caption = #1057#1090#1088#1072#1085#1072':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object CBox: TComboBox
    Left = 80
    Top = 8
    Width = 177
    Height = 21
    Align = alCustom
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 0
    Text = #1056#1086#1089#1089#1080#1103' (hh.ru)'
    Items.Strings = (
      #1056#1086#1089#1089#1080#1103' (hh.ru)'
      #1050#1072#1079#1072#1093#1089#1090#1072#1085' (hh.kz)'
      #1041#1077#1083#1072#1088#1091#1089#1100' (rabota.by)'
      #1059#1079#1073#1077#1082#1080#1089#1090#1072#1085' (hh.uz)'
      #1040#1079#1077#1088#1073#1072#1081#1076#1078#1072#1085' (hh1.az)'
      #1043#1088#1091#1079#1080#1103' (headhunter.ge)'
      #1050#1099#1088#1075#1099#1079#1089#1090#1072#1085' (headhunter.kg)')
  end
  object SaveButton: TButton
    Left = 80
    Top = 90
    Width = 177
    Height = 25
    Align = alCustom
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 1
    OnClick = SaveButtonClick
  end
  object AutorunBox: TCheckBox
    Left = 8
    Top = 35
    Width = 249
    Height = 17
    Alignment = taLeftJustify
    Caption = #1047#1072#1087#1091#1089#1082#1072#1090#1100' '#1087#1088#1080' '#1089#1090#1072#1088#1090#1077' '#1089#1080#1089#1090#1077#1084#1099
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
  end
  object MinimizeBox: TCheckBox
    Left = 8
    Top = 58
    Width = 249
    Height = 17
    Alignment = taLeftJustify
    Caption = #1057#1074#1086#1088#1072#1095#1080#1074#1072#1090#1100' '#1074' '#1090#1088#1077#1081' '#1087#1088#1080' '#1079#1072#1087#1091#1089#1082#1077
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 3
  end
end
