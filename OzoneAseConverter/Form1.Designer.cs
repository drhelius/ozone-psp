/*
* Ozone - PSP Edition
* Copyright (C) 2009-2013 Ignacio Sanchez

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program. If not, see http://www.gnu.org/licenses/
*
*/

namespace OzoneAseConverter
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
           this.button1 = new System.Windows.Forms.Button();
           this.textBox1 = new System.Windows.Forms.TextBox();
           this.button2 = new System.Windows.Forms.Button();
           this.label1 = new System.Windows.Forms.Label();
           this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
           this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
           this.checkBox1 = new System.Windows.Forms.CheckBox();
           this.groupBox1 = new System.Windows.Forms.GroupBox();
           this.groupBox1.SuspendLayout();
           this.SuspendLayout();
           // 
           // button1
           // 
           this.button1.Location = new System.Drawing.Point(334, 42);
           this.button1.Name = "button1";
           this.button1.Size = new System.Drawing.Size(75, 23);
           this.button1.TabIndex = 0;
           this.button1.Text = "Abrir ASE";
           this.button1.UseVisualStyleBackColor = true;
           this.button1.Click += new System.EventHandler(this.button1_Click);
           // 
           // textBox1
           // 
           this.textBox1.Location = new System.Drawing.Point(13, 15);
           this.textBox1.Name = "textBox1";
           this.textBox1.Size = new System.Drawing.Size(396, 20);
           this.textBox1.TabIndex = 1;
           // 
           // button2
           // 
           this.button2.Location = new System.Drawing.Point(253, 42);
           this.button2.Name = "button2";
           this.button2.Size = new System.Drawing.Size(75, 23);
           this.button2.TabIndex = 2;
           this.button2.Text = "Convertir";
           this.button2.UseVisualStyleBackColor = true;
           this.button2.Click += new System.EventHandler(this.button2_Click);
           // 
           // label1
           // 
           this.label1.AutoSize = true;
           this.label1.Enabled = false;
           this.label1.FlatStyle = System.Windows.Forms.FlatStyle.System;
           this.label1.Location = new System.Drawing.Point(75, 16);
           this.label1.Name = "label1";
           this.label1.Size = new System.Drawing.Size(247, 13);
           this.label1.TabIndex = 4;
           this.label1.Text = "Geardome 2007. Programado por Nacho Sánchez.";
           // 
           // openFileDialog1
           // 
           this.openFileDialog1.DefaultExt = "ase";
           this.openFileDialog1.Filter = "Archivos ASE (*.ase)|*.ase";
           this.openFileDialog1.ShowReadOnly = true;
           this.openFileDialog1.Title = "Abrir ASE";
           this.openFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
           // 
           // saveFileDialog1
           // 
           this.saveFileDialog1.DefaultExt = "O3D";
           this.saveFileDialog1.Filter = "Archivos O3D (*.O3D)|*.O3D";
           this.saveFileDialog1.Title = "Fichero Final O3D";
           this.saveFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.saveFileDialog1_FileOk);
           // 
           // checkBox1
           // 
           this.checkBox1.AutoSize = true;
           this.checkBox1.Location = new System.Drawing.Point(13, 42);
           this.checkBox1.Name = "checkBox1";
           this.checkBox1.Size = new System.Drawing.Size(112, 17);
           this.checkBox1.TabIndex = 5;
           this.checkBox1.Text = "Exportar Normales";
           this.checkBox1.UseVisualStyleBackColor = true;
           // 
           // groupBox1
           // 
           this.groupBox1.Controls.Add(this.label1);
           this.groupBox1.Location = new System.Drawing.Point(13, 65);
           this.groupBox1.Name = "groupBox1";
           this.groupBox1.Size = new System.Drawing.Size(396, 41);
           this.groupBox1.TabIndex = 6;
           this.groupBox1.TabStop = false;
           // 
           // Form1
           // 
           this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
           this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
           this.ClientSize = new System.Drawing.Size(423, 120);
           this.Controls.Add(this.groupBox1);
           this.Controls.Add(this.checkBox1);
           this.Controls.Add(this.button2);
           this.Controls.Add(this.textBox1);
           this.Controls.Add(this.button1);
           this.MaximizeBox = false;
           this.Name = "Form1";
           this.Text = "Ozone ASE Converter";
           this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
           this.Load += new System.EventHandler(this.Form1_Load);
           this.groupBox1.ResumeLayout(false);
           this.groupBox1.PerformLayout();
           this.ResumeLayout(false);
           this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}

