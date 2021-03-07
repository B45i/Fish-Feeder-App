import { Component } from '@angular/core';
import { MatDialog } from '@angular/material/dialog';
import { AboutDialogComponent } from './about-dialog/about-dialog.component';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss'],
})
export class AppComponent {
  constructor(private dialog: MatDialog) {}

  onInfo(): void {
    this.dialog.open(AboutDialogComponent);
  }
}
