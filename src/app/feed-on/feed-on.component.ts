import { Component, OnInit } from '@angular/core';
import { FormArray, FormBuilder, FormGroup, Validators } from '@angular/forms';
import { IFeedSettings } from '../models/dto';
import { FeederService } from '../services/feeder.service';

@Component({
  selector: 'app-feed-on',
  templateUrl: './feed-on.component.html',
  styleUrls: ['./feed-on.component.scss'],
})
export class FeedOnComponent implements OnInit {
  get useSystemTime(): boolean {
    return this.form.get('settings').get('useSystemTime').value;
  }

  get feedTimeControl(): FormArray {
    return this.form.get('feedTimes') as FormArray;
  }

  get feedTimeControls(): Array<FormGroup> {
    return this.feedTimeControl['controls'] as Array<FormGroup>;
  }

  readonly minutesValidator = [
    Validators.required,
    Validators.min(0),
    Validators.max(59),
  ];

  readonly hourValidator = [
    Validators.required,
    Validators.min(0),
    Validators.max(23),
  ];

  form = this.fb.group({
    feedTimes: this.fb.array([]),
    pumpTime: this.fb.group({
      onTime: ['', this.minutesValidator],
      offTime: ['', this.minutesValidator],
    }),
    aerationTime: this.fb.group({
      onTime: ['', this.hourValidator],
      offTime: ['', this.hourValidator],
    }),
    settings: this.fb.group({
      useSystemTime: [true],
      customTime: [''],
    }),
  });

  constructor(private fb: FormBuilder, private feederService: FeederService) {}

  ngOnInit(): void {
    this.getSavedSettings();
  }

  onNewFeedTime(): void {
    this.feedTimeControl.push(this.newFeedTime());
  }

  onRemoveFeedTime(i: number): void {
    this.feedTimeControl.removeAt(i);
  }

  getSavedSettings() {
    this.feederService.getSettings().subscribe((setting: IFeedSettings) => {
      (setting.feedTimes || []).forEach((ft) =>
        this.feedTimeControl.push(
          this.newFeedTime(ft.feedTime, ft.feedDuration)
        )
      );

      this.form.patchValue(setting);
    });
  }

  saveSettings() {
    this.feederService.saveSettings(this.form.value).subscribe();
  }

  private newFeedTime(feedTime?, feedDuration?): FormGroup {
    return this.fb.group({
      feedTime: [feedTime, this.hourValidator],
      feedDuration: [feedDuration],
    });
  }
}
